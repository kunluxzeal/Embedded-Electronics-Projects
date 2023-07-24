Imports System.IO
Imports System.IO.Ports
Imports System.Threading
Imports MySql.Data.MySqlClient

Public Class Form1

    Dim interval As Integer ' used to clear the textbox1
    Dim secs As Integer
    Dim mins As Integer
    Dim value2 As Decimal
    Dim mysqlcon As New MySqlConnection("SERVER=localhost;USERID=root;PASSWORD=;DATABASE=vb;")
    Dim mysqlcommd As New MySqlCommand
    Dim da As New MySqlDataAdapter
    Dim dt As New DataTable
    Dim i As Integer
    Dim data As String

    Dim sflag1 As Integer
    Dim sflag2 As Integer
    Dim sflag3 As Integer
    Dim sflag4 As Integer
    Dim sflag5 As Integer
    Dim sflag6 As Integer
    Dim sflag7 As Integer
    Dim sflag8 As Integer
    Dim sflag9 As Integer

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        secs = 0
        mins = 0
        interval = 0

        sflag1 = 0
        sflag2 = 0
        sflag3 = 0
        sflag4 = 0
        sflag5 = 0
        sflag6 = 0
        sflag7 = 0
        sflag8 = 0
        sflag9 = 0

        DataGridView1.RowTemplate.Height = 70
        showdata()

        SerialPort1.PortName = "COM9"
        SerialPort1.BaudRate = 9600
        SerialPort1.DataBits = 8
        SerialPort1.Parity = Parity.None
        SerialPort1.StopBits = StopBits.One
        SerialPort1.Handshake = Handshake.None
        SerialPort1.Encoding = System.Text.Encoding.Default
        SerialPort1.Open()
    End Sub

    Private Sub showdata()
        mysqlcon.Open()
        With mysqlcommd
            .Connection = mysqlcon
            .CommandText = "select * from attend"

        End With
        da.SelectCommand = mysqlcommd
        dt.Clear()
        da.Fill(dt)
        DataGridView1.DataSource = dt


        mysqlcon.Close()


    End Sub



    Private Sub btn_save_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btn_save.Click
        mysqlcon.Open()
        With mysqlcommd
            .Connection = mysqlcon
            .CommandText = "insert into attend(rollnu,name,class,present,late,absent) values('" & txt_roll.Text & "','" & txt_name.Text & "','" & txt_class.Text & "','" & txt_present.Text & "','" & txt_late.Text & "','" & txt_absent.Text & "')"
            i = .ExecuteNonQuery
        End With
        If i > 0 Then
            MsgBox("data saved:" & i & "rows")
            clear()

        End If
        mysqlcon.Close()
        showdata()


    End Sub
    Sub clear()
        txt_class.Clear()
        txt_name.Clear()
        txt_roll.Clear()


    End Sub

    Private Sub DataGridView1_CellContentClick(ByVal sender As System.Object, ByVal e As System.Windows.Forms.DataGridViewCellEventArgs) Handles DataGridView1.CellContentClick
        'txt_roll.Text = DataGridView1.Item(0, e.RowIndex).Value
        'txt_name.Text = DataGridView1.Item(1, e.RowIndex).Value
        'txt_class.Text = DataGridView1.Item(2, e.RowIndex).Value
    End Sub

    Private Sub DataGridView1_CellMouseUp(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewCellMouseEventArgs) Handles DataGridView1.CellMouseUp
        txt_roll.Text = DataGridView1.Item(0, e.RowIndex).Value
        txt_name.Text = DataGridView1.Item(1, e.RowIndex).Value
        txt_class.Text = DataGridView1.Item(2, e.RowIndex).Value
        txt_present.Text = DataGridView1.Item(3, e.RowIndex).Value
        txt_late.Text = DataGridView1.Item(4, e.RowIndex).Value
        txt_absent.Text = DataGridView1.Item(5, e.RowIndex).Value
    End Sub

    Private Sub btn_delete_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btn_delete.Click
        mysqlcon.Open()
        With mysqlcommd
            .Connection = mysqlcon
            .CommandText = "delete from attend where rollnu=" & DataGridView1.SelectedRows(0).Cells(0).Value
            i = .ExecuteNonQuery
        End With
        If i > 0 Then

            MsgBox("data deleted")
            clear()


        End If
        mysqlcon.Close()
        showdata()
    End Sub

    Private Sub btn_update_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btn_update.Click
        mysqlcon.Open()
        With mysqlcommd
            .Connection = mysqlcon
            .CommandText = "update attend set rollnu='" & txt_roll.Text & "',name='" & txt_name.Text & "',class='" & txt_class.Text & "' where rollnu=" & DataGridView1.SelectedRows(0).Cells(0).Value
            i = .ExecuteNonQuery
        End With
        If i > 0 Then

            MsgBox("data Updated")
            clear()


        End If
        mysqlcon.Close()
        showdata()
    End Sub

    Private Sub btn_search_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btn_search.Click


        mysqlcon.Open()

        With mysqlcommd
            .Connection = mysqlcon
            .CommandText = "select * from attend where rollnu like '%" & txt_search.Text & "%'"

        End With

        da.SelectCommand = mysqlcommd
        dt.Clear()
        da.Fill(dt)
        DataGridView1.DataSource = dt



        mysqlcon.Close()


    End Sub



    Private Sub DataReceived(ByVal sender As Object, ByVal e As SerialDataReceivedEventArgs) Handles SerialPort1.DataReceived


        Try

            Dim mydata As String = ""
            mydata = SerialPort1.ReadExisting()

            If TextBox1.InvokeRequired Then
                TextBox1.Invoke(DirectCast(Sub() TextBox1.Text &= mydata, MethodInvoker))

            Else
                TextBox1.Text &= mydata

            End If
        Catch ex As Exception
            MessageBox.Show(ex.Message)
        End Try
    End Sub


    Private Sub Timer4_Tick(sender As System.Object, e As System.EventArgs) Handles Timer4.Tick
        secs = secs + 1
        txt_timer.Text = secs
        If secs > 59 Then
            secs = 0
            mins = mins + 1
            txt_mins.Text = mins
        End If



    End Sub


    Private Sub CheckBox1_CheckedChanged(sender As System.Object, e As System.EventArgs) Handles CheckBox1.CheckedChanged
        CheckBox1.Checked = False

        mysqlcon.Open()
        With mysqlcommd
            .Connection = mysqlcon
            .CommandText = "update attend set present='" & txt_present.Text & "',late='" & txt_late.Text & "',absent='" & txt_absent.Text & "' where rollnu=" & DataGridView1.SelectedRows(0).Cells(0).Value
            i = .ExecuteNonQuery

        End With
        If i > 0 Then

            ' MsgBox("data Updated")
            clear()


        End If
        mysqlcon.Close()
        showdata()
        txt_absent.Text = ""
        txt_present.Text = ""
        txt_late.Text = ""


    End Sub

    Private Sub Timer5_Tick(sender As System.Object, e As System.EventArgs) Handles Timer5.Tick
        ' for present 
        If ((mins <= 3) And (InStr(TextBox1.Text, "1001")) And (sflag1 = 0)) Then
            txt_present.Text = "Present"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag1 = 1
        End If

        If mins <= 3 And InStr(TextBox1.Text, "1002") And sflag2 = 0 Then
            txt_present.Text = "Present"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag2 = 1
        End If

        If mins <= 3 And InStr(TextBox1.Text, "1003") And sflag3 = 0 Then
            txt_present.Text = "Present"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag3 = 1
        End If

        If mins <= 3 And InStr(TextBox1.Text, "1004") And sflag4 = 0 Then
            txt_present.Text = "Present"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag4 = 1
        End If

        If mins <= 3 And InStr(TextBox1.Text, "1005") And sflag5 = 0 Then
            txt_present.Text = "Present"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag5 = 1
        End If

        If mins <= 3 And InStr(TextBox1.Text, "1006") And sflag6 = 0 Then
            txt_present.Text = "Present"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag6 = 1
        End If

        If mins <= 3 And InStr(TextBox1.Text, "1007") And sflag7 = 0 Then
            txt_present.Text = "Present"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag7 = 1
        End If

        If mins <= 3 And InStr(TextBox1.Text, "1008") And sflag8 = 0 Then
            txt_present.Text = "Present"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag8 = 1
        End If

        If mins <= 3 And InStr(TextBox1.Text, "1009") And sflag9 = 0 Then
            txt_present.Text = "Present"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag9 = 1
        End If

        ' for late 
        If mins > 3 And mins < 6 And InStr(TextBox1.Text, "1001") And sflag1 = 0 Then
            txt_late.Text = "late"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag1 = 1
        End If

        If mins > 3 And mins < 6 And InStr(TextBox1.Text, "1002") And sflag2 = 0 Then
            txt_late.Text = "late"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag2 = 1
        End If

        If mins > 3 And mins < 6 And InStr(TextBox1.Text, "1003") And sflag3 = 0 Then
            txt_late.Text = "late"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag3 = 1
        End If

        If mins > 3 And mins < 6 And InStr(TextBox1.Text, "1004") And sflag4 = 0 Then
            txt_late.Text = "late"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag4 = 1
        End If

        If mins > 3 And mins < 6 And InStr(TextBox1.Text, "1005") And sflag5 = 0 Then
            txt_late.Text = "late"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag5 = 1
        End If

        If mins > 3 And mins < 6 And InStr(TextBox1.Text, "1006") And sflag6 = 0 Then
            txt_late.Text = "late"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag6 = 1
        End If

        If mins > 3 And mins < 6 And InStr(TextBox1.Text, "1007") And sflag7 = 0 Then
            txt_late.Text = "late"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag7 = 1
        End If

        If mins > 3 And mins < 6 And InStr(TextBox1.Text, "1008") And sflag8 = 0 Then
            txt_late.Text = "late"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag8 = 1
        End If

        If mins > 3 And mins < 6 And InStr(TextBox1.Text, "1009") And sflag9 = 0 Then
            txt_late.Text = "late"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag9 = 1
        End If

        ' for absent
        If mins >= 6 And mins <= 10 And InStr(TextBox1.Text, "1001") And sflag1 = 0 Then
            txt_absent.Text = "absent"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag1 = 1
        End If

        If mins >= 6 And mins <= 10 And InStr(TextBox1.Text, "1002") And sflag2 = 0 Then
            txt_absent.Text = "absent"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag2 = 1
        End If

        If mins >= 6 And mins <= 10 And InStr(TextBox1.Text, "1003") And sflag3 = 0 Then
            txt_absent.Text = "absent"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag3 = 1
        End If

        If mins >= 6 And mins <= 10 And InStr(TextBox1.Text, "1004") And sflag4 = 0 Then
            txt_absent.Text = "absent"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag4 = 1
        End If

        If mins >= 6 And mins <= 10 And InStr(TextBox1.Text, "1005") And sflag5 = 0 Then
            txt_absent.Text = "absent"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag5 = 1
        End If

        If mins >= 6 And mins <= 10 And InStr(TextBox1.Text, "1006") And sflag6 = 0 Then
            txt_absent.Text = "absent"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag6 = 1
        End If

        If mins >= 6 And mins <= 10 And InStr(TextBox1.Text, "1007") And sflag7 = 0 Then
            txt_absent.Text = "absent"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag7 = 1
        End If

        If mins >= 6 And mins <= 10 And InStr(TextBox1.Text, "1008") And sflag8 = 0 Then
            txt_absent.Text = "absent"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag8 = 1
        End If

        If mins >= 6 And mins <= 10 And InStr(TextBox1.Text, "1009") And sflag9 = 0 Then
            txt_absent.Text = "absent"
            CheckBox1.Checked = True
            TextBox1.Text = ""
            sflag9 = 1
        End If


    End Sub



    Private Sub Button1_Click(sender As System.Object, e As System.EventArgs) Handles Button1.Click
        TextBox1.Text = ""
    End Sub


    Private Sub TextBox1_TextChanged(sender As System.Object, e As System.EventArgs) Handles TextBox1.TextChanged

        Try
            value2 = Convert.ToDecimal(TextBox1.Text)

        Catch ex As Exception
            TextBox1.Text = ""
        End Try
        txt_search.Text = value2
        mysqlcon.Open()

        With mysqlcommd
            .Connection = mysqlcon
            .CommandText = "select * from attend where rollnu like '%" & txt_search.Text & "%'"

        End With

        da.SelectCommand = mysqlcommd
        dt.Clear()
        da.Fill(dt)
        DataGridView1.DataSource = dt
        mysqlcon.Close()


    End Sub


    Private Sub Button2_Click(sender As System.Object, e As System.EventArgs) Handles Button2.Click
        Me.Close()
        password.Close()

    End Sub
End Class
