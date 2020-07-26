import pandas
import sys
import numpy

# TODO: change the flowing values in real application
bookNumToDisplay = 30
intrSizeDiscussed = 5

bookLists = ['humanities', 'childrenLiterature', 'fiction', 'history', 'psychology', 'JapaneseLiterature',
             'classicalLiterature', 'biography', 'military', 'masterpieces', 'essays', 'networkNovels', 'martialArts',
             'classics', 'poetry', 'youth', 'healthy', 'scienceFiction', 'travel', 'lightFiction', 'philosophy', 'love',
             'health', 'suspense', 'reasoning', 'education', 'programming', 'photography', 'magic', 'ChineseLiterature']
chinabookLists = ['人文', '儿童文学', '小说', '历史', '心理学', '日本文学', '古典文学', '传记', '军事', '名著', '杂文',
                  '网络小说', '武侠', '经典', '诗歌', '青春', '养生', '科幻', '穿越', '轻小说', '哲学', '爱情', '健康',
                  '悬疑', '推理', '教育', '编程', '摄影', '魔幻', '中国文学']
book_database = 'ct1.csv'
user_database = 'userInfo1.csv'
userHobby = 'userLike.csv'
weight = 100

df = pandas.read_csv(book_database, encoding='utf-8', low_memory=False)
df = df.dropna(axis=0, how='all')
df = df.fillna(0)
rows_sum = df.shape[0]
angle_dict = {}


def return_str(like):
    if like == 0:
        return 'Dislike'
    if like == 1:
        return 'Like'
    else:
        sys.stderr.write('意味がない')


def return_tagLists_accordingToBookID(bookID):
    tagLists = []
    for tag in bookLists:
        if df.loc[bookID, tag] > 0:
            tagLists.append(tag)
    return tagLists


def updateUserDatabase(userList):
    if len(userList) != intrSizeDiscussed:
        sys.stderr.write("说好30个的")
        exit()
    elif len(userList[0]) != 3:
        sys.stderr.write("要（userID, bookID, 0/1）哦！")
        exit()
    for userID_bookID_like in userList:
        userID, bookID, like = userID_bookID_like
        book_index = df[df['id'] == int(bookID)].index.tolist()[0]
        updateSomeUserHobby(userID, bookID, int(like))
        changeSomeUserPreferenceWeight(userID, return_tagLists_accordingToBookID(book_index), int(like))


def updateSomeUserHobby(userID, bookID=0, like=-1):
    hobby = pandas.read_csv(userHobby, encoding='utf-8', low_memory=False)
    hobby = hobby.dropna(axis=0, how='all')
    is_new_user = True
    for u_name in hobby['userName'].tolist():
        if userID == u_name:
            is_new_user = False
    if bookID == 0 and like == -1 and not is_new_user:
        return is_new_user
    if is_new_user:
        hobby = hobby.append([{'userName': userID}])
        hobby = hobby.dropna(axis=0, how='all')
        if bookID == 0 and like == -1:
            hobby.to_csv(userHobby, encoding="utf_8_sig", index=False)
            return is_new_user
        else:
            sys.stderr.write('你小子没有初始化！！！\n')
            sys.stderr.write('必须通过 initialSomeUserPreferenceWeight(str::userName, list::tagList) 来初始化新用户')
            exit()
    else:
        pass
    islike = return_str(like)
    if not hobby.loc[hobby.userName == userID, islike].isnull().bool():
        likeStart = hobby.loc[hobby.userName == userID, islike]
        likeStart = likeStart.iloc[0]
        if type(likeStart) == numpy.float64:
            likeStart = str(int(likeStart))
        else:
            likeStart = str(likeStart)
        likeStartList = likeStart.split('|')
        likeStartList.append(str(bookID))
        likeStartList = list(set(likeStartList))
        likeStr = '|'.join(likeStartList)
        hobby.loc[hobby.userName == userID, islike] = likeStr
    else:
        likeStr = str(bookID)
        hobby.loc[hobby.userName == userID, islike] = likeStr
    hobby.to_csv(userHobby, encoding="utf_8_sig", index=False)


def returnUserPreferenceList(user_name):
    hobby = pandas.read_csv(userHobby, encoding='utf-8', low_memory=False)
    hobby = hobby.dropna(axis=0, how='all')
    user_data = pandas.read_csv('userInfo1.csv', encoding='utf-8', low_memory=False)
    user_row = user_data[user_data['userName'] == user_name]
    user_data = user_row.append([user_row] * (rows_sum - 1))
    x_sum = numpy.zeros(rows_sum)
    y_sum = numpy.zeros(rows_sum)
    mol_sum = numpy.zeros(rows_sum)
    for tag in bookLists:
        mol_sum = mol_sum + user_data[tag].values * df[tag].values
        x_sum = x_sum + user_data[tag].values ** 2
        y_sum = y_sum + df[tag].values ** 2
    consine_vector = mol_sum / numpy.sqrt(x_sum) / numpy.sqrt(y_sum)
    consine_vector = list(consine_vector)
    index_cos = range(1, rows_sum)
    consine_dict = dict(zip(index_cos, consine_vector))
    ordered_angle_list = sorted(consine_dict.items(), key=lambda item: item[1], reverse=True)
    user_preference_list = []
    likeList = []
    dislikeList = []
    if not hobby.loc[hobby.userName == user_name, 'Like'].isnull().bool():
        likeList = hobby.loc[hobby.userName == user_name, 'Like'].iloc[0].split('|')
    if not hobby.loc[hobby.userName == user_name, 'Dislike'].isnull().bool():
        dislikeList = hobby.loc[hobby.userName == user_name, 'Dislike'].iloc[0].split('|')
    bookNumToSearch = bookNumToDisplay + len(likeList) + len(dislikeList)
    for book_index in ordered_angle_list[:bookNumToSearch]:
        user_preference_list.append(str(book_index[0]))
    result_1 = list(set(user_preference_list) - set(likeList + dislikeList))
    return result_1[:bookNumToDisplay]


def changeSomeUserPreferenceWeight(userName='kewudgsj', tagList=[], likeORnot=1):
    """change some user's tag value"""
    ud = pandas.read_csv(user_database, encoding='utf-8')
    for classTag in tagList:
        ud.loc[ud.userName == userName, classTag] = distinctLikeORnot(ud.loc[ud.userName == userName, classTag], weight,
                                                                      likeORnot)
    ud.to_csv(user_database, encoding="utf_8_sig", index=False)


def distinctLikeORnot(ini, change_step, dis):
    if dis == 1:
        return ini + change_step
    elif dis == 0:
        return ini - change_step
    else:
        sys.stderr.write('要输入0或1哦！')


def initialSomeUserPreferenceWeight(userName, tagList=[]):
    updateSomeUserHobby(userName)
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
    pass
