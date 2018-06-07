import urllib
import json
import requests
import boto3
import datetime
from encodings.utf_8 import decode

def lambda_handler(event, context):
   
    try:
        dynamodb = boto3.resource('dynamodb', region_name='ap-northeast-1')
        table    = dynamodb.Table('XXXXXX')
        response = table.scan()
        
    except:
        raise Exception("Internal error. 'dynamodb'.")

    print('----------------------------')
    today = datetime.datetime.today()
    for num in range(response['ScannedCount']):
        limitDay = datetime.datetime.strptime(response['Items'][num]['token-limit'], "%Y/%m/%d %H:%M:%S")
        if (limitDay - today).days <= 5: # 5日前になったら更新
            print('5日前')
           
            params = {
                'token': response['Items'][num]['token']
            }
            akashiResponse = requests.post('https://atnd.ak4.jp/api/cooperation/token/reissue/xxxxxxx', data=params)
            if akashiResponse.status_code == 200:
                j = akashiResponse.json()
                DynamoWrite(response['Items'][num]['id'],response['Items'][num]['name'],j['response']['token'],j['response']['expired_at'])
    print('----------------------------')
    return response                

def DynamoWrite(id,name,token,tokenLimit):
    print('--- token update ' + id +' ' +name +' ' +token+' ' +tokenLimit)

    try:
        dynamodb = boto3.resource('dynamodb', region_name='ap-northeast-1')
        table    = dynamodb.Table('xxxxxx')
        res1 = table.put_item(
            Item={
                'id':id,
                'name':name,
                'token':token,
                'token-limit':tokenLimit
            }
        )
    except:
        raise Exception("Internal error. 'DynamoWrite() '.")
 