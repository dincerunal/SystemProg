package org.csystem.upperclient

import android.app.Activity
import android.os.AsyncTask
import android.os.Bundle
import android.os.Handler
import android.os.Message
import android.view.Menu
import android.view.View
import android.widget.*
import org.csystem.net.SocketUtil
import org.csystem.util.ByteConverter
import java.io.DataInputStream
import java.io.DataOutputStream
import java.net.Socket

class MainActivity : Activity(), CompoundButton.OnCheckedChangeListener {
    private lateinit var m_editTextMessage: EditText
    private lateinit var m_textViewMessage: TextView
    private lateinit var m_buttonSend: Button
    private lateinit var m_toggleButtonConnect: ToggleButton
    private lateinit var m_socket: Socket
    private lateinit var m_dos: DataOutputStream
    private lateinit var m_dis: DataInputStream

    private var m_handlerConnection : Handler = object : Handler() {
        override fun handleMessage(msg: Message?)
        {
            var resultMsg: String = "Connected"

            if (msg?.what == -1) {
                resultMsg = "Connection problem"
                return
            }

            m_textViewMessage.text = resultMsg
            m_buttonSend.isEnabled = true
        }
    }

    private inner class SendMessageTask : AsyncTask<String, Void, String>() {
        override fun doInBackground(vararg params: String?): String
        {
            var result = ""

            try {
                var msg = params[0] as String

                var bufLen = ByteConverter.getBytes(msg.length)
                var buf = msg.toByteArray()

                SocketUtil.write(m_dos, bufLen)
                SocketUtil.write(m_dos, buf)
                if (msg.equals("quit"))
                    return "Connection closed"

                SocketUtil.read(m_dis, bufLen)
                var len = ByteConverter.toInt(bufLen)

                SocketUtil.read(m_dis, buf, 0, len)

                result = String(buf)
            }
            catch (ex:Throwable) {

            }

            return result
        }

        override fun onPostExecute(result: String?)
        {
            m_textViewMessage.text = result
        }
    }

    private fun initialize()
    {
        initControls()
    }

    private fun initControls()
    {
        m_editTextMessage = findViewById(R.id.MAINACTIVITY_EDITTEXT_MESSAGE)
        m_textViewMessage = findViewById(R.id.MAINACTIVITY_TEXTVIEW_MESSAGE)
        m_buttonSend = findViewById(R.id.MAINACTIVITY_BUTTON_SEND)
        m_toggleButtonConnect = findViewById(R.id.MAINACTIVITY_TOGGLEBUTTON_CONNECT)

        m_toggleButtonConnect.setOnCheckedChangeListener(this)
    }

    private fun connectToServer()
    {
        Thread(Runnable {
            var what = 0
            try {
                m_socket = Socket("172.20.29.186", 5120)
                m_dos = DataOutputStream(m_socket.getOutputStream())
                m_dis = DataInputStream(m_socket.getInputStream())
            } catch (ex:Throwable) {
                what = -1
            }

            m_handlerConnection.sendEmptyMessage(what)
        }).start()
    }

    private fun disconnectServer()
    {
        try {
            m_socket.close()
            m_dis.close()
            m_dos.close()
            m_textViewMessage.text = "disconnected"
            m_buttonSend.isEnabled = false
        }
        catch (ex:Throwable) {
            Toast.makeText(this, ex.message, Toast.LENGTH_LONG).show()
        }
    }

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        initialize()
    }

    override fun onDestroy()
    {
        super.onDestroy()
        disconnectServer()
    }

    override fun onCheckedChanged(buttonView: CompoundButton?, isChecked: Boolean)
    {
        Toast.makeText(this, if (isChecked) "ON" else "OFF", Toast.LENGTH_LONG).show()

        if (isChecked)
            connectToServer()
        else
            disconnectServer()
    }

    fun onSendButtonClicked(ex:View)
    {
        SendMessageTask().execute(m_editTextMessage.text.toString())
    }
}
