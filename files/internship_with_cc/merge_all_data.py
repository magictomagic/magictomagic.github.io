import pandas

bookLists = ['人文', '儿童文学', '小说', '历史', '心理学', '日本文学', '古典文学', '传记', '军事', '名著',
             '杂文', '网络小说', '武侠', '经典', '诗歌', '青春', '养生', '科幻', '穿越', '轻小说', '哲学', '爱情', '健康',
             '悬疑', '推理', '教育', '编程', '摄影', '魔幻', '中国文学']

weight = 100

for tag in bookLists:
    df = pandas.read_csv('./new_washed/%s.csv' % tag, encoding='utf-8')
    df = df.dropna(axis=0, how='all')
    print("***********" + tag + "********************")
    for row_tag_index, row_tag in df.iterrows():
        data_result = pandas.read_csv('new_merge.csv', encoding='utf-8')
        data_result = data_result.dropna(axis=0, how='all')
        result_rows = data_result.shape[0]
        # print(row_tag['标题'])
        flag = False
        for row_result_index, row_result in data_result.iterrows():
            if row_tag['标题'] == row_result['标题'] and row_tag['pub'] == row_result['pub']:
                data_result.loc[row_result_index, tag] = weight
                flag = True
                data_result.to_csv('new_merge.csv', encoding='utf-8-sig', index=False)
        if not flag:
            result_rows = result_rows + 1
            data_result.loc[result_rows] = row_tag
            data_result.loc[result_rows, tag] = weight
            data_result.to_csv('new_merge.csv', encoding='utf-8-sig', index=False)

