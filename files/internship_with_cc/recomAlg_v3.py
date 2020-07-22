import pandas
import math
import sys
import time
bookLists = ['人文', '儿童文学', '小说', '历史', '心理学', '日本文学', '古典文学', '传记', '军事', '名著',
             '杂文', '网络小说', '武侠', '经典', '诗歌', '青春', '养生', '科幻', '穿越', '轻小说', '哲学', '爱情', '健康',
             '悬疑', '推理', '教育', '编程', '摄影', '魔幻', '中国文学']
book_database = 'new_merge.csv'
user_database = 'userInfo1.csv'
weight = 100

df = pandas.read_csv(book_database, encoding='utf-8', low_memory=False)
df = df.dropna(axis=0, how='all')
df = df.fillna(0)

# TODO: change these values in real application
bookNumToDisplay = 20
intrSizeDiscussed = 3


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
        changeSomeUserPreferenceWeight(userID, return_tagLists_accordingToBookID(int(bookID)), like)


def returnUserPreferenceList(user_name):
    user_data = pandas.read_csv(user_database, encoding='utf-8', low_memory=False)
    user_vector = user_data.loc[user_data['userName'] == user_name]
    angle_dict = {}
    for row_tag_index, row_tag in df.iterrows():
        mol_sum = 0
        x_sum = 0
        y_sum = 0
        for tag in bookLists:
            mol_sum = mol_sum + int(user_vector[tag]) * int(row_tag[tag])
            x_sum = x_sum + int(user_vector[tag]) ** 2
            y_sum = y_sum + int(row_tag[tag]) ** 2
        angle = mol_sum / math.sqrt(x_sum) / math.sqrt(y_sum)
        angle_dict[row_tag_index] = angle
    ordered_angle_list = sorted(angle_dict.items(), key=lambda item: item[1], reverse=True)
    user_preference_list = []
    for book_index in ordered_angle_list[:bookNumToDisplay]:
        user_preference_list.append(book_index[0])
    return user_preference_list


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


# TODO: if user not exist, creat a user line
def initialSomeUserPreferenceWeight(userName, tagList=[]):
    """change some user's tag value"""
    ud = pandas.read_csv(user_database, encoding='utf-8')
    for classTag in tagList:
        ud.loc[ud.userName == userName, classTag] = weight
    ud.to_csv(user_database, encoding="utf_8_sig", index=False)


if __name__ == '__main__':
    # test data
    gsj = ['sbgsj', '3', 1]
    wct = ['sbwct', '2', 1]
    hhy = ['hhyzuishuai', '1', 0]
    interface_2 = [gsj, wct, hhy]
    test_tagList = ['小说', '历史']

    # method demo
    time_start1 = time.time()
    initialSomeUserPreferenceWeight('hhyzuishuai', test_tagList)
    updateUserDatabase(interface_2)
    time_end1 = time.time()
    print('time cost', time_end1 - time_start1, 's')
    time_start = time.time()
    print(returnUserPreferenceList('hhyzuishuai'))
    time_end = time.time()
    print('returnUserPreferenceList time cost', time_end - time_start, 's')

