namespace Kirillov_lab1_sharp
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.btn_start = new System.Windows.Forms.Button();
            this.btn_stop = new System.Windows.Forms.Button();
            this.listbox_threads = new System.Windows.Forms.ListBox();
            this.label_Nthreads = new System.Windows.Forms.Label();
            this.textBox_Nthreads = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // btn_start
            // 
            this.btn_start.Location = new System.Drawing.Point(12, 57);
            this.btn_start.Name = "btn_start";
            this.btn_start.Size = new System.Drawing.Size(148, 68);
            this.btn_start.TabIndex = 0;
            this.btn_start.Text = "Start";
            this.btn_start.UseVisualStyleBackColor = true;
            this.btn_start.Click += new System.EventHandler(this.btn_start_Click);
            // 
            // btn_stop
            // 
            this.btn_stop.Location = new System.Drawing.Point(12, 131);
            this.btn_stop.Name = "btn_stop";
            this.btn_stop.Size = new System.Drawing.Size(148, 68);
            this.btn_stop.TabIndex = 1;
            this.btn_stop.Text = "Stop";
            this.btn_stop.UseVisualStyleBackColor = true;
            this.btn_stop.Click += new System.EventHandler(this.btn_stop_Click);
            // 
            // listbox_threads
            // 
            this.listbox_threads.FormattingEnabled = true;
            this.listbox_threads.ItemHeight = 16;
            this.listbox_threads.Location = new System.Drawing.Point(166, 12);
            this.listbox_threads.Name = "listbox_threads";
            this.listbox_threads.Size = new System.Drawing.Size(153, 276);
            this.listbox_threads.TabIndex = 2;
            // 
            // label_Nthreads
            // 
            this.label_Nthreads.AutoSize = true;
            this.label_Nthreads.Location = new System.Drawing.Point(9, 9);
            this.label_Nthreads.Name = "label_Nthreads";
            this.label_Nthreads.Size = new System.Drawing.Size(133, 17);
            this.label_Nthreads.TabIndex = 3;
            this.label_Nthreads.Text = "Создать N потоков";
            // 
            // textBox_Nthreads
            // 
            this.textBox_Nthreads.Location = new System.Drawing.Point(12, 29);
            this.textBox_Nthreads.Name = "textBox_Nthreads";
            this.textBox_Nthreads.Size = new System.Drawing.Size(100, 22);
            this.textBox_Nthreads.TabIndex = 4;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(447, 376);
            this.Controls.Add(this.textBox_Nthreads);
            this.Controls.Add(this.label_Nthreads);
            this.Controls.Add(this.listbox_threads);
            this.Controls.Add(this.btn_stop);
            this.Controls.Add(this.btn_start);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btn_start;
        private System.Windows.Forms.Button btn_stop;
        private System.Windows.Forms.ListBox listbox_threads;
        private System.Windows.Forms.Label label_Nthreads;
        private System.Windows.Forms.TextBox textBox_Nthreads;
    }
}

