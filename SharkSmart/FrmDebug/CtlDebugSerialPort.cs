using System;
using System.Drawing;
using System.IO.Ports;
using System.Text;
using System.Windows.Forms;

namespace SharkSmart
{
    public partial class CtlDebugSerialPort : UserControl
    {
        private UpdateText updateText;
        private string[] sLines;
        private string[] sNewLines;
        private int received_count;
        public bool IsComOpened = false;

        public CtlDebugSerialPort()
        {
            InitializeComponent();
            RefreshPort();
        }

        private void RefreshPort()
        {
            tscbPort.Items.Clear();
            //检查是否含有串口  
            string[] str = SerialPort.GetPortNames();
            if (str == null)
            {
                tscbPort.Text = "没有串口";
                return;
            }

            //添加串口项目  
            foreach (string s in SerialPort.GetPortNames())
            {//获取有多少个COM口  
                tscbPort.Items.Add(s);
            }

            //串口设置默认选择项  
            tscbPort.SelectedIndex = tscbPort.Items.Count - 1;
        }



        private void tsbOpenCom_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
            {
                try
                {
                    //设置各“串口设置” 
                    serialPort1.PortName = tscbPort.Text;                           //串口号
                    serialPort1.BaudRate = Convert.ToInt32(tscbBaudRate.Text);      //波特率  
                    serialPort1.DataBits = 8;                                       //数据位  
                    serialPort1.StopBits = StopBits.One;
                    serialPort1.Parity = Parity.None;
                    if (serialPort1.IsOpen)//如果打开状态，则先关闭一下  
                    {
                        serialPort1.Close();
                    }
                    serialPort1.Open();     //打开串口
                    serialPort1.DataReceived += SerialPort1_DataReceived;

                    tscbPort.Enabled = false;
                    tscbBaudRate.Enabled = false;
                    tsbOpenCom.Text = "关闭串口";
                    tsbOpenCom.BackColor = Color.Red;

                    this.IsComOpened = true;
                }
                catch (Exception)
                {
                    this.IsComOpened = false;
                    return;
                }
            }
            else
            {
                //恢复控件功能  
                //设置必要控件可用  
                tscbPort.Enabled = true;
                tscbBaudRate.Enabled = true;

                serialPort1.Close();    //关闭串口  
                serialPort1.DataReceived -= SerialPort1_DataReceived;
                received_count = 0;
                tsbOpenCom.Text = "打开串口";
                tsbOpenCom.BackColor = SystemColors.ActiveCaption;

                this.IsComOpened = false;
            }
        }

        //enum sendFormat { ASCII, HEX }
        //enum recvFormat { ASCII, HEX }
        //sendFormat send_format = sendFormat.ASCII;
        //recvFormat recv_format = recvFormat.ASCII;
        //StringBuilder builder;
        private void SerialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int n = serialPort1.BytesToRead;//先记录下来，避免某种原因，人为的原因，操作几次之间时间长，缓存不一致  
            if (n == 0) return;
            byte[] buf = new byte[n];//声明一个临时数组存储当前来的串口数据  
            received_count += n;//增加接收计数  
            serialPort1.Read(buf, 0, n); ;//读取缓冲数据 

            string str = Encoding.GetEncoding("UTF-8").GetString(buf);
            Engine.Logger.Debug("{0}-{1}", "串口打印", str);
        }

        private void UpdateTextRtbConsole(Color color, string text)
        {
            this.rtbConsole.SelectionColor = color;
            this.rtbConsole.AppendText(text);
            this.rtbConsole.ScrollToCaret();
            sLines = this.rtbConsole.Lines;
            if (sLines.Length > 200)
            {
                sNewLines = new string[sLines.Length - 51];
                Array.Copy(sLines, 51, sNewLines, 0, sNewLines.Length);
                this.rtbConsole.Lines = sNewLines;
            }
        }

        private void LoggerOutputStream(string text)
        {
            if (text.Contains("ERROR"))
            {
                this.rtbConsole.BeginInvoke(updateText, Color.Red, text);
            }
            else if (text.Contains("WARN"))
            {
                this.rtbConsole.BeginInvoke(updateText, Color.Yellow, text);
            }
            else
            {
                this.rtbConsole.BeginInvoke(updateText, Color.White, text);
            }
        }

        private void CtlDebugSerialPort_Load(object sender, EventArgs e)
        {
            Engine.Logger.OutputStreamEvent += new EVTechnology.Common.Logging.OutputStreamHandler(LoggerOutputStream);
            updateText += new UpdateText(UpdateTextRtbConsole);
        }
    }
}
