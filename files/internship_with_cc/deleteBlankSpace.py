import pandas

bookLists = {'人文', '儿童文学', '小说', '历史', '心理学', '日本文学', '古典文学', '传记', '军事', '名著',
             '杂文', '网络小说', '武侠', '经典', '诗歌', '青春', '养生', '科幻', '穿越', '轻小说', '哲学', '爱情', '健康',
             '悬疑', '推理', '教育', '编程', '摄影', '魔幻', '中国文学'}

for bookType in bookLists:
    print('./new_csv/豆瓣图书标签_ %s.csv' % bookType)
    df = pandas.read_csv('./new_csv/豆瓣图书标签_ %s.csv' % bookType, encoding='utf-8')
    df.drop_duplicates(subset=['标题', 'pub'], keep='first', inplace=True)

    df['标题'] = df['标题'].astype('str')
    df['标题'] = df['标题'].map(str.strip) # 字段 编号

    df['pub'] = df['pub'].astype('str')
    df['pub'] = df['pub'].map(str.strip)

    df['图片'] = df['图片'].astype('str')
    df['图片'] = df['图片'].map(str.strip)

    df['信息_链接'] = df['信息_链接'].astype('str')
    df['信息_链接'] = df['信息_链接'].map(str.strip)

    df.to_csv('./new_washed/%s.csv' % bookType, encoding="utf_8_sig", index=False)


# print(df)
