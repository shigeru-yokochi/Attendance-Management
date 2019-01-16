import urllib
import json
import requests
import boto3
import datetime
from encodings.utf_8 import decode

def lambda_handler(event, context):
    # TODO implement
#    import boto3
#    import json    
#    import datetime
    
    try:
        dynamodb = boto3.resource('dynamodb', region_name='ap-northeast-1')
        table    = dynamodb.Table('xxxxx')
        response = table.scan()
        #table    = dynamodb.Table('xxxxx')
        #res1 = table.get_item(
        #    Key={
        #        'id':'xxxxx'
        #    }
        #)
        #for item in table.scan('foo')['Items']:
        #    print item

        
    except:
        #pass
        #return json.dumps(res1)
        #return -1
        raise Exception("Internal error. 'dynamodb'.")

    print('----------------------------> start')
    today = datetime.datetime.today()
    print('now time')
    print(today)
    for num in range(response['ScannedCount']):
        print('↓')
        print('{}'.format(response['Items'][num]['name']))
        print('token-limit(str)')
        print('{}'.format(response['Items'][num]['token-limit']))
        limitDay = datetime.datetime.strptime(response['Items'][num]['token-limit'], "%Y/%m/%d %H:%M:%S")
        print('token-limit(val)')
        print(limitDay)
        if (limitDay - today).days <= 5: # 5日前になったら更新
            print('+++ 残5日以下（token再発行必要）')
            params = {
                'token': response['Items'][num]['token']
                #'token':'xxxxxx-xxxxx-xxxx-xxxxx-xxxxx'
            }
            akashiResponse = requests.post('https://atnd.ak4.jp/api/cooperation/token/reissue/xxxxx', data=params)
            if akashiResponse.status_code == 200:
                j = akashiResponse.json()
                print('{}'.format(j['response']['token']))
                print('{}'.format(j['response']['expired_at']))
                print(akashiResponse.text)
                DynamoWrite(response['Items'][num]['id'],response['Items'][num]['name'],j['response']['token'],j['response']['expired_at'])
    print('----------------------------< end')
    return response                
    #return 'OK'

def DynamoWrite(id,name,token,tokenLimit):
    print('--- token update ' + id +' ' +name +' ' +token+' ' +tokenLimit)

    try:
        dynamodb = boto3.resource('dynamodb', region_name='ap-northeast-1')
        table    = dynamodb.Table('xxxxx')
        res1 = table.put_item(
            Item={
                'id':id,
                'name':name,
                'token':token,
                'token-limit':tokenLimit
                #'token-limit': "2018/04/30 02:00:00"
            }
        )
    except:
        raise Exception("Internal error. 'DynamoWrite() '.")
    
    
