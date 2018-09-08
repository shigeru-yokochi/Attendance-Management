# Attendance-Management
Raspberry Pi とNFCタグリーダを使い勤怠管理


![image](https://user-images.githubusercontent.com/12773136/43672910-aad60824-97f3-11e8-8200-b27eb52755a3.jpg)

# 概要

AKASHIの出退勤システムに対してAKASHIのAPIをコールして出退勤の打刻を実現するもの
本コードは装置内のプログラム部であり、ユーザデータ用のDB管理についての説明は割愛しています。


# 構成
NFCタグリーダからIDを呼び出しそのIDに紐づくAKASHIユーザで
APIを実行する。
AWSのDynamoDB+APIGAteway+Lambdaを使用


# Raspberry Pi上へのインストール方法

- cd /root
- git clone https://github.com/shigeru-yokochi/Attendance-Management.git
- cd Attendance-Management

## vi define.py  (アクセスキーの設定)

```
X_API_KEY = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
AWS_API_KEY = "xxxxxxxxxxx"
```
## make

- make

## cron設定　（監視設定）

```
* * * * * /root/Attendance-Management/chkProsess.sh
```

## 実行権の付与
- chmod 755 nfc-tagtool.py 
- chmod 755 chkProsess.sh

## 自動起動設定

vi /etc/rc.local
以下を追加

```
/root/Attendance-Management/tactsw-led &
/root/Attendance-Management/nfc-tagtool.py -l &
```


# 参照
https://qiita.com/shigeru-yokochi/items/a91c0a3cb1fbb7cafafe


