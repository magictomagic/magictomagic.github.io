import pandas
import math
import sys
import numpy
import time
# TODO: change these values in real application
bookNumToDisplay = 20
intrSizeDiscussed = 5

bookLists = ['humanities', 'childrenLiterature', 'fiction', 'history', 'psychology', 'JapaneseLiterature',
             'classicalLiterature', 'biography', 'military', 'masterpieces', 'essays', 'networkNovels', 'martialArts',
             'classics', 'poetry', 'youth', 'healthy', 'scienceFiction', 'travel', 'lightFiction', 'philosophy', 'love',
             'health', 'suspense', 'reasoning', 'education', 'programming', 'photography', 'magic', 'ChineseLiterature']
chinabookLists = ['人文', '儿童文学', '小说', '历史', '心理学', '日本文学', '古典文学', '传记', '军事', '名著',
             '杂文', '网络小说', '武侠', '经典', '诗歌', '青春', '养生', '科幻', '穿越', '轻小说', '哲学', '爱情', '健康',
             '悬疑', '推理', '教育', '编程', '摄影', '魔幻', '中国文学']
book_database = 'ct.csv'
user_database = 'userInfo1.csv'
userHobby = 'userLike.csv'
weight = 100
df = pandas.read_csv(book_database, encoding='utf-8', low_memory=False)
df = df.dropna(axis=0, how='all')
df = df.fillna(0)
hobby = pandas.read_csv(userHobby, encoding='utf-8', low_memory=False)
hobby = hobby.dropna(axis=0, how='all')
angle_dict = {}

def return_str(like):
    if like == 0:
        return 'Dislike'
    if like == 1:
        return 'Like'


def return_tagLists_accordingToBookID(bookID):
    tagLists = []
    for tag in bookLists:
        if df.loc[bookID, tag] > 0:
            tagLists.append(tag)
    return tagLists


def updateUserDatabase(userList):
    if len(userList) != intrSizeDiscussed:
        print("说好30个的")
        exit()
    elif len(userList[0]) != 3:
        print("要（userID, bookID, 0/1）哦！")
        exit()
    for userID_bookID_like in userList:
        userID, bookID, like = userID_bookID_like
        book_index = df[df['id'] == int(bookID)].index.tolist()[0]
        # TODO: change to parallel. After change read from .csv to mysql
        updateSomeUserHobby(userID, bookID, int(like))  # 非阻塞方式运行
        changeSomeUserPreferenceWeight(userID, return_tagLists_accordingToBookID(book_index), int(like))


def updateSomeUserHobby(userID, bookID, like):
    islike = return_str(like)
    if not hobby.loc[hobby.userName == userID, islike].isnull().bool():
        likeStart = hobby.loc[hobby.userName == userID, islike]
        likeStart = likeStart.iloc[0]
        if type(likeStart) == numpy.float64:
            likeStart = str(int(likeStart))
        else:
            pass
        likeStartList = likeStart.split('|')
        likeStartList.append(str(bookID))
        likeStartList = list(set(likeStartList))
        likeStr = '|'.join(likeStartList)
        hobby.loc[hobby.userName == userID, islike] = likeStr
    else:
        likeStr = str(bookID)
        hobby.loc[hobby.userName == userID, islike] = likeStr
    hobby.to_csv(userHobby, encoding="utf_8_sig", index=False)


def my_cosine_similarity(user_vector, row_tag, row_tag_index):
    mol_sum = 0
    x_sum = 0
    y_sum = 0
    for tag in bookLists:
        mol_sum = mol_sum + int(user_vector[tag]) * int(row_tag[tag])
        x_sum = x_sum + int(user_vector[tag]) ** 2
        y_sum = y_sum + int(row_tag[tag]) ** 2
    angle = mol_sum / math.sqrt(x_sum) / math.sqrt(y_sum)
    angle_dict[row_tag_index] = angle

# TODO: parallel it. Next task
def returnUserPreferenceList(user_name):
    user_data = pandas.read_csv(user_database, encoding='utf-8', low_memory=False)
    user_vector = user_data.loc[user_data['userName'] == user_name]

    for row_tag_index, row_tag in df.iterrows():
        my_cosine_similarity(user_vector, row_tag, row_tag_index)

    ordered_angle_list = sorted(angle_dict.items(), key=lambda item: item[1], reverse=True)
    print(ordered_angle_list)
    user_preference_list = []
    likeList = []
    dislikeList = []
    if not hobby.loc[hobby.userName == user_name, 'Like'].isnull().bool():
        # print(hobby.loc[hobby.userName == user_name, 'Dislike'].iloc[0])
        likeList = hobby.loc[hobby.userName == user_name, 'Like'].iloc[0].split('|')
    if not hobby.loc[hobby.userName == user_name, 'Dislike'].isnull().bool():
        dislikeList = hobby.loc[hobby.userName == user_name, 'Dislike'].iloc[0].split('|')
    bookNumToSearch = bookNumToDisplay + len(likeList) + len(dislikeList)
    for book_index in ordered_angle_list[:bookNumToSearch]:
        user_preference_list.append(str(df.loc[book_index[0], 'id']))
    return list(set(user_preference_list) - set(likeList + dislikeList))


def changeSomeUserPreferenceWeight(userName='sbwct', tagList=[], likeORnot=1):
    """change some user's tag value"""
    ud = pandas.read_csv(user_database, encoding='utf-8')
    for classTag in tagList:
        ud.loc[ud.userName == userName, classTag] = distinctLikeORnot(ud.loc[ud.userName == userName, classTag], weight, likeORnot)
    ud.to_csv(user_database, encoding="utf_8_sig", index=False)


def distinctLikeORnot(ini, change_step, dis):
    if dis == 1:
        return ini + change_step
    elif dis == 0:
        return ini - change_step
    else:
        sys.stderr.write('要输入0或1哦！')


"""
    如果一开始用户没有在存用户向量的表中，则通过 test_tagList 初始化用户的向量
    change some user's tag value, only can be invoke when user confirm their initial choice
"""
def initialSomeUserPreferenceWeight(userName, tagList=[]):

    ud = pandas.read_csv(user_database, encoding='utf-8')
    isNewUser = True
    for uName in ud['userName'].tolist():
        if userName == uName:
            isNewUser = False
    if isNewUser:
        ud = ud.append([{'userName': userName}])
        ud = ud.dropna(axis=0, how='all')
        ud = ud.fillna(0)
    else:
        pass
    for classTag in transLate(tagList):
        ud.loc[ud.userName == userName, classTag] = weight
    ud.to_csv(user_database, encoding="utf_8_sig", index=False)
    return isNewUser


def transLate(tagList):
    english_tag = []
    for tag in tagList:
        english_tag.append(bookLists[chinabookLists.index(tag)])
    return english_tag

if __name__ == '__main__':
    # test data
    gsj = ['sbgsj', '3', '1']
    hatefulgsj = ['sbgsj', '6', '1']
    wct = ['sbgsj', '9', '1']
    hhy_smart = ['sbgsj', '67', '0']
    hhy = ['sbgsj', '24', '0']
    interface_2 = [gsj, wct, hhy, hatefulgsj, hhy_smart]
    test_tagList = ['网络小说', '武侠', '经典']


    print(hobby)
    # updateUserDatabase(interface_2)
    start = time.time()
    # 区别：乱序
    print(returnUserPreferenceList('sbgsj'))
    # ['42', '16', '21', '41', '58', '880', '12', '884', '47', '25', '14', '31', '40', '64', '32', '765', '10', '4', '51',
     # '54', '152', '44']
    print(hobby)
    end = time.time()

    print('Time taken in seconds -', end - start)


