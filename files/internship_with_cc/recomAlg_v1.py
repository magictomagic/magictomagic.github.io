import pandas
import random

def randGet(pathName, BookNumber):
    someTypeOfBookLists = pandas.read_csv(pathName)
    rowNum = someTypeOfBookLists.shape[0]
    displayBookList = []
    i = 0
    while len(displayBookList) < BookNumber:
        print(len(displayBookList))
        row = random.randint(0, rowNum)
        displayBookList.append(someTypeOfBookLists.iloc[row])
    return displayBookList


# def __init__(self, userData = 'userInfo1.csv'):
#     self.bookLists = ['人文', '儿童文学', '小说', '历史', '心理学', '日本文学', '古典文学', '传记', '军事', '名著',
#          '杂文', '网络小说', '武侠', '经典', '诗歌', '青春', '养生', '科幻', '穿越', '轻小说', '哲学', '爱情', '健康',
#          '悬疑', '推理', '教育', '编程', '摄影', '魔幻']
#     self.userIDlist = pandas.read_csv(userData)


def fromWho(userName = 'sbwct', userData = 'userInfo1.csv'):
    """前端调用 this api to get user's preference bookList"""
    bookLists = ['人文', '儿童文学', '小说', '历史', '心理学', '日本文学', '古典文学', '传记', '军事', '名著',
                      '杂文', '网络小说', '武侠', '经典', '诗歌', '青春', '养生', '科幻', '穿越', '轻小说', '哲学', '爱情', '健康',
                      '悬疑', '推理', '教育', '编程', '摄影', '魔幻']
    userIDlist = pandas.read_csv(userData)
    sum_all = 0
    for tag in bookLists:
        sum_all = sum_all + userIDlist[userIDlist['userName'].isin([userName])].iloc[0].at[tag]

    # TODO: if not arrive displayBookNum, random set 0 to 1
    """book number of each class"""
    bookNum = {}
    displayBookNum = 20
    for tag in bookLists:
        bookNum[tag] = int(displayBookNum * userIDlist[userIDlist['userName'].isin([userName])].iloc[0].at[tag] / sum_all)
    print(bookNum)

    # TODO: eliminate duplication
    """get all display lists"""
    # gstbl = getSomeTypeOfBookLists()
    result = []
    for tag in bookLists:
        tmpList = randGet.randGet('./washed/%s.csv' % tag, bookNum[tag])
        result = result + tmpList
    return result

def changeSomeUserPreferenceWeight(userData = 'userInfo1.csv', userName = 'sbwct', classTag = '小说', weight = 400):
    """change some user's tag value"""
    ud = pandas.read_csv(userData, encoding='utf-8')
    ud.loc[ud.userName == userName, classTag] = weight
    ud.to_csv(userData, encoding="utf_8_sig", index=False)

if __name__=='__main__':
    # test = apiGetBookLists()

    # print(test.fromWho())
    changeSomeUserPreferenceWeight('userInfo1.csv', 'sbwct', '小说', 400)
    # print(len(test.fromWho()))
