# Attendance-Management
Raspberry Pi とNFCタグリーダを使い勤怠管理

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


