# Attendance-Management
Raspberry Pi とNFCタグリーダを使い勤怠管理


![image](https://user-images.githubusercontent.com/12773136/43672910-aad60824-97f3-11e8-8200-b27eb52755a3.jpg)

# 概要

AKASHIの出退勤システムに対してAKASHIのAPIをコールして出退勤の打刻を実現するもの

# 構成
NFCタグリーダからIDを呼び出しそのIDに紐づくAKASHIユーザで
APIを実行する。
AWSのDynamoDB+APIGAteway+LAMBDAを使用

# アクセスキー設定が必要な箇所
nfc-tagtool.py


# 参照
https://qiita.com/shigeru-yokochi/items/a91c0a3cb1fbb7cafafe


