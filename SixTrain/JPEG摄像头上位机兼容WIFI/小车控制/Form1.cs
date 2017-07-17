using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;
using System.Timers;
using System.Net.Sockets;
using System.Net;  

namespace 小车控制
{
    public partial class Form1 : Form
    {
        int fps = 0;
        int ReadDataBytes = 0;
        Byte[] readBytes;
        Image jpg_image;
        Socket acceptSocket;
        Socket listenSocket;
        Thread thread1;

        static Queue<Byte []> data_list = new Queue<Byte []>();            //列队用于缓冲串口数据

        private Thread serverThread;   // 启动服务线程  

        public Form1()
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;
            timer1.Start();
            timer2.Start();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

            radioButton3.Checked = true;
            textBox3.Text = "7777";
            textBox2.Text = "192.168.191.1";
            button6.Enabled = true;
            button2.Enabled = false;

            #region  波特率初始化
            string[] BaudRate = { "9600", "115200", "315200" ,"921600"};
            //让下拉框只能选择已有选项，不能输入
            comboBox2.DropDownStyle = ComboBoxStyle.DropDownList;
            for (int i = 0; i < BaudRate.Length; i++)
            {
                comboBox2.Items.Add(BaudRate[i]);
            }
            comboBox2.Text = "115200";
            serialPort1.BaudRate = 115200;
            #endregion

            #region 串口初始化
            
            //让下拉框只能选择已有选项，不能输入
            comboBox1.DropDownStyle = ComboBoxStyle.DropDownList;
            string[] portList = System.IO.Ports.SerialPort.GetPortNames();      //搜索可用串口
            for (int i = 0; i < portList.Length; i++)
            {
                string name = portList[i];
                comboBox1.Items.Add(name);
            }

            try
            {
                comboBox1.SelectedIndex = 0;
            }
            catch
            {
                MessageBox.Show("没有可用端口");
            }
            //串口接受消息响应设置
            serialPort1.DataReceived += serialPort1_DataReceived;
            #endregion
        }

        private void Data_Process()             //串口数据处理函数
        {
            int Position = 0;                   //标记位置
            int sta = 0;                        //标记检测状态

            Byte[] jpg_buf = new Byte[102400];   //缓冲存储数组，可增大以适应更大的图像

            while (true)
            {
                if (data_list.Count > 0)
                {
                    Byte[] buffer = data_list.Dequeue();

                    if (sta == 1)   //已经检测到了帧头
                    {
                        for (int i = 0; i < buffer.Length - 1; i++)
                        {
                            if (buffer[i] == 0XFF && buffer[i + 1] == 0XD9)
                            {
                                Array.Copy(buffer, 0, jpg_buf, Position, i + 2);
                                jpg_image = Make_Image(jpg_buf);   //数组转换为JPEG图像

                                if (jpg_image!=null)
                                {
                                    if (jpg_image.RawFormat.Equals(System.Drawing.Imaging.ImageFormat.Jpeg))
                                    {
                                        fps++;

                                    }
                                    else
                                    {
                                        MessageBox.Show("数组转换为JPEG过程出错");
                                    }

                                }
                                Position = 0;
                                sta = 0;
                            }
                        }
                        if (sta == 1)   //检测到了帧头但未检测到帧尾，即可用内容
                        {
                            Array.Copy(buffer, 0, jpg_buf, Position, buffer.Length);
                            Position += buffer.Length;
                        }
                    }
                    if (sta == 0)    //还未检测到帧头
                    {
                        if (buffer.Length>1)
                        {
                            for (int i = 0; i < buffer.Length - 1; i++)
                            {
                                if (buffer[i] == 0XFF && buffer[i + 1] == 0XD8)
                                {
                                    Array.Copy(buffer, i, jpg_buf, Position, buffer.Length - i);
                                    Position += (buffer.Length - i);

                                    sta = 1;   //检测到了帧头
                                }
                            }
                        }

                    }
                }
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e) //串口改变
        {
            serialPort1.PortName = comboBox1.Text;
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e) //串口收到数据
        {
            readBytes = new Byte[serialPort1.BytesToRead];

            if (serialPort1.BytesToRead > 0)
            {
                ReadDataBytes = this.serialPort1.Read(readBytes, 0, readBytes.Length);

                data_list.Enqueue(readBytes);
            }
        }

        private void Form1_KeyUp(object sender, KeyEventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                serialPort1.Write("St\r\n");
            }
            else
            {
                MessageBox.Show("请先打开串口！");
            }
        }

        public static Image Make_Image(Byte[] buffer)                         //数组到图像函数
        {
            MemoryStream ms = new MemoryStream(buffer);
            if(ms!=null)
            {
                try
                {
                    Image image = System.Drawing.Image.FromStream(ms);
                    return image;
                }
                catch
                {
                    //MessageBox.Show("数据出错");
                }
            }
            return null;
        }

        private void timer1_Elapsed(object sender, ElapsedEventArgs e)        //timer1用于刷新图像
        {
            timer1.Enabled = false;

            if (fps > 0)
            {
                try
                {
                    pictureBox1.Image = jpg_image;
                }
                catch
                {
                    MessageBox.Show("图像错误");
                }

            }

            timer1.Enabled = true;
        }

        private void timer2_Elapsed(object sender, ElapsedEventArgs e)         //timer2用于统计帧率
        {
            timer2.Enabled = false;

            textBox1.Text = Convert.ToString(fps/3.111);
            fps = 0;

            timer2.Enabled = true;
        }

        private void button6_Click_1(object sender, EventArgs e)  //控制串口及后台队列线程打开关闭
        {
            if (!serialPort1.IsOpen)
            {
                try
                {
                    thread1 = new Thread(new ThreadStart(Data_Process));         //建立一个线程用于处理串口接收到的数据
                    thread1.IsBackground = true;
                    thread1.Start();

                    serialPort1.Open();

                    button6.Text = "关闭串口";
                    comboBox1.Enabled = false;
                }
                catch
                {
                    MessageBox.Show("串口打开失败了！可能是串口不存在或已被占用。");
                }
            }
            else
            {
                serialPort1.Close();
                if (thread1.IsAlive)
                {
                    thread1.Abort();
                }   
                button6.Text = "打开串口";
                comboBox1.Enabled = true;
            }
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)  // 更改波特率
        {
            //设置波特率
            serialPort1.BaudRate = int.Parse(comboBox2.Text);
        }

        private void button1_Click(object sender, EventArgs e) //刷新串口
        {
            //清空之前的选项
            comboBox1.Items.Clear();

            //自动搜索并且添加可用串口
            string[] PortName = SerialPort.GetPortNames();//读取
            for (int i = 0; i < PortName.Length; i++)//添加
            {
                comboBox1.Items.Add(PortName[i]);
            }

            try
            {
                comboBox1.SelectedIndex = 0;
            }
            catch
            {
                MessageBox.Show("没有可用端口");
            }
        }

        private void button2_Click(object sender, EventArgs e) //开启串口处理及WIFI接收线程
        {
            button2.Enabled = false;
            button3.Enabled = true;
            thread1 = new Thread(new ThreadStart(Data_Process));         //建立一个线程用于处理串口接收到的数据
            thread1.IsBackground = true;
            thread1.Start();
            serverThread = new Thread(new ThreadStart(Listen));
            serverThread.IsBackground = true;
            serverThread.Start(); 
        }
        private void Listen()  //socket监听
        {
            try
            {
                IPAddress ipAddress = IPAddress.Parse(textBox2.Text);
                Int32 port = Int32.Parse(textBox3.Text);
                listenSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                listenSocket.Bind(new IPEndPoint(ipAddress, port));
                listenSocket.Listen(1);
                acceptSocket = listenSocket.Accept();                
                while (true)
                {
                    Receive(acceptSocket);
                }  

            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message, "Server提示");
            }

        }


        private void Receive(Socket socket) //socket接收数据入队
        {
            byte[] buffer = new byte[1024];
            int length = socket.Receive(buffer) ;
            try
            {
                if (length > 0)
                {
                    data_list.Enqueue(buffer);
                }
            }
            catch 
            { 

            }
            
        }

        private static void DestroySocket(Socket socket) //销毁socket
        {
            if (socket.Connected)
            {
                socket.Shutdown(SocketShutdown.Both);
            }
            socket.Close();
        }  

        private void radioButton2_CheckedChanged(object sender, EventArgs e) //WIFI与串口切换
        {
            if (radioButton2.Checked)
            {
                try
                {
                    serialPort1.Close();
                    button6.Text = "打开端口";
                }
                catch
                {
                    MessageBox.Show("串口已关闭");
                }
                button6.Enabled = false;
                button2.Enabled = true;
            }
            if (radioButton3.Checked)
            {
                button3_Click(null, null);

                button2.Enabled = false;
                button6.Enabled = true;
            }
        }


        private void button3_Click(object sender, EventArgs e) //关闭socket，停止线程
        {
            try
            {
                    // 停止服务后，允许 启动服务，但是不允许发送消息  
                    button2.Enabled = true;
                    button3.Enabled = false;
                    if (acceptSocket != null)
                    {
                        DestroySocket(acceptSocket);
                    }
                    if (listenSocket != null)
                    {
                        DestroySocket(listenSocket);
                    }   
                    if (serverThread.IsAlive)
                    {
                        serverThread.Abort();
                    }
                    if (thread1.IsAlive)
                    {
                        thread1.Abort();
                    }                     
                    
            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message, "Server提示");
            }  
        }

    }
}
