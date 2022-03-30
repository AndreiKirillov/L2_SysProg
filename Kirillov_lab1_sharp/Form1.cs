using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;
using System.IO;
using System.Runtime.InteropServices;

namespace Kirillov_lab1_sharp
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct header
    {
        [MarshalAs(UnmanagedType.I4)]
        public int thread_id;
        [MarshalAs(UnmanagedType.I4)]
        public int message_size;
    };

    public partial class Form1 : Form
    {
        [DllImport("FileMapping.dll", SetLastError = true, EntryPoint = "_CreateMappingFile@0", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        private static extern bool CreateMappingFile();//StringBuilder filename);
       
        [DllImport("FileMapping.dll", EntryPoint = "_SendMappingMessage@8", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        unsafe private static extern bool SendMappingMessage(IntPtr message, ref header h);
        
        [DllImport("FileMapping.dll", EntryPoint = "_ReadMessage@4", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        private static extern StringBuilder ReadMessage(ref header h);

        [DllImport("FileMapping.dll", EntryPoint = "_CloseFileMapping@0", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        private static extern void CloseFileMapping();



        private Process child_process = null;
        private int count = 0;
        private System.Threading.EventWaitHandle stopEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "CloseThread");
        private System.Threading.EventWaitHandle startEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "CreateNewThread");
        private System.Threading.EventWaitHandle confirmEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "ConfirmEvent");
        private System.Threading.EventWaitHandle closeProgrammEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "CloseProgrammEvent");
        private System.Threading.EventWaitHandle messageEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "SendMessage");

        public Form1()
        {
            InitializeComponent();
            //CreateMappingFile();
            //IntPtr message = Marshal.StringToHGlobalAnsi("Fuckin message");
            //header h = new header();
            //h.thread_id = 5;
            //h.message_size = message.ToString().Length;
            //SendMappingMessage(message, ref h);
        }

        private void btn_start_Click(object sender, EventArgs e)
        {
            if(child_process == null || child_process.HasExited)
            {
                listbox_threads.Items.Clear();
                count = 0;
                if (textBox_Nthreads.TextLength == 0)
                {
                    MessageBox.Show("Внимание! Задайте количество создаваемых потоков!");
                    return;
                }

                child_process = Process.Start("C:/repository/SysProg/L2_SysProg/Debug/Kirillov_lab1_cpp.exe");
                listbox_threads.Items.Add("Все потоки");
                listbox_threads.Items.Add("Главный поток");
                int nThreads = Convert.ToInt32(textBox_Nthreads.Text);
                if (confirmEvent.WaitOne())
                    if (nThreads > 0)
                    {
                        for (int i = 0; i < nThreads; i++)
                        {
                            Thread.Sleep(100);   // небольшая задержка, иначе потоки не могут нормально инициализироваться
                            startEvent.Set();
                            if(confirmEvent.WaitOne(-1))
                                listbox_threads.Items.Add($"{++count}-й поток");
                        }
                    }
            }
            else
            {
                if (textBox_Nthreads.TextLength == 0)
                {
                    MessageBox.Show("Внимание! Задайте количество создаваемых потоков!");
                    return;
                }
                int nThreads = Convert.ToInt32(textBox_Nthreads.Text);
                if (nThreads > 0)
                {
                    for (int i = 0; i < nThreads; i++)
                    {
                        Thread.Sleep(100);
                        startEvent.Set();
                        confirmEvent.WaitOne();
                        listbox_threads.Items.Add($"{++count}-й поток");
                    }
                }
            }
            
        }

        private void btn_stop_Click(object sender, EventArgs e)
        {
            if (child_process != null)
            {
                stopEvent.Set();
                if(count == 0)
                {
                    if (closeProgrammEvent.WaitOne())
                    {
                        listbox_threads.Items.Clear();
                        count = 0;
                        child_process.Close();
                        child_process = null;
                    }
                }
                else
                {
                    if (confirmEvent.WaitOne())
                    {
                        listbox_threads.Items.RemoveAt(count + 1);
                        --count;
                    }
                }
            }
            else 
            {
                MessageBox.Show("Ошибка! Дочерняя программа не запущена!");
            }
        }

        private void btn_send_Click(object sender, EventArgs e)
        {
            if (child_process != null)
            {
                if (textBox_Message.TextLength == 0)
                {
                    MessageBox.Show("Внимание! Напишите текст сообщения!");
                    return;
                }
                var filename = new StringBuilder("myfile.dat");

                //CreateMappingFile(filename);
                if (!CreateMappingFile())//filename))
                {
                    MessageBox.Show("Внимание! Не удалось открыть файл для передачи сообщения!");
                    return;
                }
                //unsafe
                //{
                //    void* msg;
                //    IntPtr message = Marshal.StringToHGlobalAnsi("Fuckin message");
                //    //IntPtr message = new StringBuilder(textBox_Message.Text);
                //    header h = new header();
                //    h.thread_id = listbox_threads.SelectedIndex - 1;
                //    h.message_size = message.ToString().Length;
                //    msg = message.ToPointer();

                //    if (!SendMappingMessage(msg, ref h))
                //    {
                //        CloseFileMapping();
                //        MessageBox.Show("Внимание! Не удалось отправить сообщения!");
                //        return;
                //    }
                //}

                IntPtr message = Marshal.StringToHGlobalAnsi("messageffffffffffffff");
                //StringBuilder message = new StringBuilder(textBox_Message.Text);
                header h = new header();
                h.thread_id = listbox_threads.SelectedIndex - 1;
                h.message_size = message.ToString().Length;

                if (!SendMappingMessage(message, ref h))
                {
                    CloseFileMapping();
                    MessageBox.Show("Внимание! Не удалось отправить сообщения!");
                    return;
                }

                messageEvent.Set();

                confirmEvent.WaitOne();
            }
            else
            {
                MessageBox.Show("Ошибка! Дочерняя программа не запущена!");
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            closeProgrammEvent.Set();
            if(child_process != null)
                child_process.Close();
        }

    }
}
