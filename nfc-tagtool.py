#サンプルプログラム tagtool.pyをnfc-tagtool.py として　def show_tag(self, tag):にコードを追加する
    def show_tag(self, tag):
        #-------------ここから追加------------------------
        print("--- start")
        print(tag)
        print(vars(tag))
        tagId = "".join(format(x, '02x') for x in tag._nfcid)
        print(tagId)
        #タクトスイッチ状態LED状態read
        f = open("/tmp/tactsw-led.tmp","rb")
        ledPos =  f.read(2)     
        f.close()
        try:
            print("ledPos:"+ledPos)
            data = {
                'userId': tagId,
                'statusCode': ledPos
            }
            headers = {
                'Content-type': 'application/json',
                'x-api-key': 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'
            }
            response = requests.post('https://xxxxxxxxxxxxxxxx.execute-api.ap-northeast-1.amazonaws.com/prod/nfc-id',data=json.dumps(data),headers=headers)
            #print(json.dumps(dict(response.headers)))
            print(response.status_code)
            if response.status_code == 200:
                #タッチ完了
                f = open("/tmp/beep.tmp","w")
                val = f.write("1")     
                f.close()
            else:
                #タッチNG
                f = open("/tmp/alm.tmp","w")
                val = f.write("1")     
                f.close()

            time.sleep(1)

        except:
            print("*** AWS requests failed.")
            #タッチNG
            f = open("/tmp/alm.tmp","w")
            val = f.write("1")     
            f.close()

        print("--- end")
        #------------ここまで----------------------------------
        if tag.ndef:


 