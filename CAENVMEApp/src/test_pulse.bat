rem
rem   To write          CaenTalk  -w  card  address  value         # all values in hex
rem   To read          CaenTalk card address    # all values in hex
rem
:loop
"C:\Labview modules\CAEN v895\CaenTalk.exe" -w  0  4c   1
sleep 1
goto loop