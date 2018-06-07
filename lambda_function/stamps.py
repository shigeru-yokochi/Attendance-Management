import urllib
import json
import requests
from encodings.utf_8 import decode

def lambda_handler(event, context):
    # TODO implement
    import boto3
    import json    

    if event['userId'] == "":
        raise Exception("Parameter error. Missing field 'userId'.")

    if event['statusCode'] == "":
        raise Exception("Parameter error. Missing field 'statusCode'.")

    try:
        dynamodb = boto3.resource('dynamodb', region_name='ap-northeast-1')
        table    = dynamodb.Table('xxxxxx')
        res1 = table.get_item(
            Key={
                'id': event['userId']
            }
        )
    except:
        raise Exception("Internal error. 'dynamodb'.")

     
    if json.dumps(res1).find('token') == -1:
        raise Exception("Parameter error. token Not found 'userId'.")


    if event['statusCode'] == '0':
        statusCode='11'
    elif event['statusCode'] == '1':
        statusCode='12'
    elif event['statusCode'] == '2':
        statusCode='31'
    elif event['statusCode'] == '3':
        statusCode='32'
    else:
        raise Exception("Parameter error. Missing value 'statusCode 0..3'.")        
    
        
    params = (
        ('token', res1['Item']['token']),
        ('type', statusCode)
    )
    response = requests.post('https://atnd.ak4.jp/api/cooperation/xxxxxxx/stamps', data=params)
    
    if response.status_code != 200:
        raise Exception("Parameter error. Akashi API(1).")

    j = response.json()
    if j['success'] == 'false':
        raise Exception("Parameter error. Akashi API(2).")
                    
    return response                
