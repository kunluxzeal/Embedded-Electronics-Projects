Option Explicit
Dim BUFFERS$
Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Dim message As Integer
Dim search As String
Dim strTest As String
Dim user1 As Integer
Dim user2 As Integer



Dim time1 As Integer
Dim time2 As Integer

Dim mins As Integer
Dim secs As Integer

Dim teacherflag As Integer
Dim sflag1 As Integer ' student 1 flag
Dim sflag2 As Integer ' student 2 flag
Dim sflag3 As Integer ' student 3 flag


Dim msending As Integer ' message sending flag

Dim absent1 As Integer
Dim absent2 As Integer
Dim absent3 As Integer


Dim absentflag As Integer

Dim flag1 As Integer
Dim flag2 As Integer
Dim flag3 As Integer

Dim counter As Integer



Private Sub Form_Load()
mins = 0
Timer3.Enabled = False
teacherflag = 0
sflag1 = 0
sflag2 = 0
sflag3 = 0


msending = 0

absent1 = 0
absent2 = 0
absent3 = 0


absentflag = 0

flag1 = 0
flag2 = 0
flag3 = 0

counter = 0
'MSComm1.Settings = "9600,n,8,1"      'Change this with the Baud rate of your modem (The one you use with Hyper Terminal)
'MSComm1.CommPort = 10                   ' Change this with the port your Modem is attached,(eg bluetooth)
'MSComm1.PortOpen = True

With MSComm1
        .CommPort = 3
        .Settings = "9600,N,8,1"
        .Handshaking = comRTS
        .RTSEnable = True
        .DTREnable = True
        .RThreshold = 1
        .SThreshold = 1
        .InputMode = comInputModeText
        .InputLen = 0
        .PortOpen = True 'must be the last
    End With
    



End Sub






Private Sub Timer1_Timer()


txtrec.Text = MSComm1.Input
search = txtrec.Text
If (InStr(search, "teacher") And (teacherflag = 0)) Then   ' when the teacher will swipe the rfid card the word teacher will be sent to the computer
Check3.Value = 1
Timer3.Enabled = True ' timer3 is used for timing mins and secs
teacherflag = 1
Timer4.Enabled = True
End If

If (InStr(search, "stu1") And (sflag1 = 0) And mins < 6) Then    ' then the student1 will be marked present
Check4.Value = 1
sflag1 = 1
End If

If (InStr(search, "stu2") And (sflag2 = 0) And mins < 6) Then    ' then the student1 will be marked present
Check5.Value = 1
sflag2 = 1
End If

If (InStr(search, "stu3") And (sflag3 = 0) And mins < 6) Then    ' then the student1 will be marked present
Check6.Value = 1
sflag3 = 1
End If





End Sub






Private Sub Timer2_Timer()
Label5.Caption = Time

End Sub

Private Sub Timer3_Timer()
secs = secs + 1
Label7.Caption = secs

If secs > 59 Then
secs = 0
mins = mins + 1
Label3.Caption = mins
End If



End Sub

Private Sub Timer4_Timer()
' sending message to students to let them know that the teacher has arrived.
' in the controller program each and every user will have its own message sending function in which his number will be specified.
counter = counter + 1
Label3.Caption = counter

If ((teacherflag = 1) And ((counter > 10) And (counter < 20)) And (Check8.Value = 0)) Then
Check8.Value = 1
End If

If ((teacherflag = 1) And ((counter > 30) And (counter < 40)) And (Check9.Value = 0)) Then
Check9.Value = 1
End If


If ((teacherflag = 1) And ((counter > 50) And (counter < 60)) And (Check10.Value = 0)) Then
Check10.Value = 1
End If



If ((teacherflag = 1) And ((counter > 90) And (counter < 120)) And (Check12.Value = 0)) Then ' this when checked means all messages are sent
Check12.Value = 1
End If

' check box for parents messages

If ((Check12.Value = 1) And ((counter > 300) And (counter < 320) And (Check4.Value = 0))) Then ' this when checked means all messages are sent
Check13.Value = 1
End If

If ((Check12.Value = 1) And ((counter > 330) And (counter < 350) And (Check5.Value = 0))) Then ' this when checked means all messages are sent
Check14.Value = 1
End If

If ((Check12.Value = 1) And ((counter > 370) And (counter < 380) And (Check6.Value = 0))) Then ' this when checked means all messages are sent
Check15.Value = 1
End If

If ((Check12.Value = 1) And ((counter > 390) And (counter < 400) And (Check7.Value = 0))) Then ' this when checked means all messages are sent
Check16.Value = 1
End If

'*************************************

If ((Check8.Value = 1) And (flag1 = 0)) Then

MSComm1.Output = "a"
flag1 = 1
End If

If ((Check9.Value = 1) And (flag2 = 0)) Then

MSComm1.Output = "b"
flag2 = 1
End If

If ((Check10.Value = 1) And (flag3 = 0)) Then

MSComm1.Output = "c"
flag3 = 1
End If



' messages sending to their parents


If ((Check13.Value = 1) And (absent1 = 0)) Then

MSComm1.Output = "l"
absent1 = 1
End If

If ((Check14.Value = 1) And (absent2 = 0)) Then

MSComm1.Output = "m"
absent2 = 1
End If

If ((Check15.Value = 1) And (absent3 = 0)) Then

MSComm1.Output = "n"
absent3 = 1
End If

End Sub
