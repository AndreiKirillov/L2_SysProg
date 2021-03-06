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
        // подключаем dll функцию отправки сообщения

        [DllImport("FileMapping.dll", EntryPoint = "_SendMappingMessage@8", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        unsafe private static extern bool SendMappingMessage(IntPtr message, ref header h);


        private Process child_process = null;
        private int count_threads = 0;
        private System.Threading.EventWaitHandle stopEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "CloseThread");
        private System.Threading.EventWaitHandle startEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "CreateNewThread");
        private System.Threading.EventWaitHandle confirmEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "ConfirmEvent");
        private System.Threading.EventWaitHandle closeProgrammEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "CloseProgrammEvent");
        private System.Threading.EventWaitHandle messageEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "SendMessage");

        public Form1()
        {
            InitializeComponent();
        }

        private void btn_start_Click(object sender, EventArgs e)
        {
            if(child_process == null || child_process.HasExited)
            {
                listbox_threads.Items.Clear();
                count_threads = 0;
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
                            startEvent.Set();
                            if(confirmEvent.WaitOne(-1))
                                listbox_threads.Items.Add($"{++count_threads}-й поток");
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
                        startEvent.Set();
                        confirmEvent.WaitOne();
                        listbox_threads.Items.Add($"{++count_threads}-й поток");
                    }
                }
            }
            
        }

        private void btn_stop_Click(object sender, EventArgs e)
        {
            if (child_process != null)
            {
                stopEvent.Set();
                if(count_threads == 0)
                {
                    if (closeProgrammEvent.WaitOne())
                    {
                        listbox_threads.Items.Clear();
                        count_threads = 0;
                        child_process.Close();
                        child_process = null;
                    }
                }
                else
                {
                    if (confirmEvent.WaitOne())
                    {
                        listbox_threads.Items.RemoveAt(count_threads + 1);
                        --count_threads;
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
                if(listbox_threads.SelectedIndex < 0)
                {
                    MessageBox.Show("Внимание! Выберете поток!");
                    return;
                }

                IntPtr message = Marshal.StringToHGlobalAnsi(textBox_Message.Text);
                header h = new header();
                h.thread_id = listbox_threads.SelectedIndex - 1;
                string msg = Marshal.PtrToStringAnsi(message);
                h.message_size = msg.Length + 1;

                if (!SendMappingMessage(message, ref h))
                {
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
