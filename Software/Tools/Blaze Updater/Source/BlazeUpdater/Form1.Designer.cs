namespace BlazeUpdater
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.PortNameList = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.connect = new System.Windows.Forms.Button();
            this.refresh = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label3 = new System.Windows.Forms.Label();
            this.status = new System.Windows.Forms.Label();
            this.progress = new System.Windows.Forms.ProgressBar();
            this.erase = new System.Windows.Forms.Button();
            this.program = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // PortNameList
            // 
            this.PortNameList.FormattingEnabled = true;
            this.PortNameList.Location = new System.Drawing.Point(91, 18);
            this.PortNameList.Name = "PortNameList";
            this.PortNameList.Size = new System.Drawing.Size(78, 21);
            this.PortNameList.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(25, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(60, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Port Name:";
            // 
            // connect
            // 
            this.connect.Location = new System.Drawing.Point(320, 16);
            this.connect.Name = "connect";
            this.connect.Size = new System.Drawing.Size(78, 24);
            this.connect.TabIndex = 2;
            this.connect.Text = "Connect";
            this.connect.UseVisualStyleBackColor = true;
            this.connect.Click += new System.EventHandler(this.connect_Click);
            // 
            // refresh
            // 
            this.refresh.Location = new System.Drawing.Point(175, 16);
            this.refresh.Name = "refresh";
            this.refresh.Size = new System.Drawing.Size(78, 24);
            this.refresh.TabIndex = 9;
            this.refresh.Text = "Refresh";
            this.refresh.UseVisualStyleBackColor = true;
            this.refresh.Click += new System.EventHandler(this.refresh_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.status);
            this.groupBox1.Controls.Add(this.progress);
            this.groupBox1.Controls.Add(this.erase);
            this.groupBox1.Controls.Add(this.program);
            this.groupBox1.Location = new System.Drawing.Point(12, 59);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(407, 127);
            this.groupBox1.TabIndex = 10;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Bootloader";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 83);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(51, 13);
            this.label3.TabIndex = 13;
            this.label3.Text = "Progress:";
            // 
            // status
            // 
            this.status.AutoSize = true;
            this.status.Location = new System.Drawing.Point(353, 83);
            this.status.Name = "status";
            this.status.Size = new System.Drawing.Size(22, 13);
            this.status.TabIndex = 12;
            this.status.Text = "OK";
            // 
            // progress
            // 
            this.progress.Location = new System.Drawing.Point(79, 79);
            this.progress.Name = "progress";
            this.progress.Size = new System.Drawing.Size(268, 23);
            this.progress.TabIndex = 11;
            // 
            // erase
            // 
            this.erase.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.erase.Location = new System.Drawing.Point(16, 30);
            this.erase.Name = "erase";
            this.erase.Size = new System.Drawing.Size(92, 24);
            this.erase.TabIndex = 10;
            this.erase.Text = "Erase";
            this.erase.UseVisualStyleBackColor = true;
            this.erase.Click += new System.EventHandler(this.erase_Click);
            // 
            // program
            // 
            this.program.Location = new System.Drawing.Point(233, 30);
            this.program.Name = "program";
            this.program.Size = new System.Drawing.Size(114, 24);
            this.program.TabIndex = 9;
            this.program.Text = "Program Firmware";
            this.program.UseVisualStyleBackColor = true;
            this.program.Click += new System.EventHandler(this.program_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(431, 200);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.refresh);
            this.Controls.Add(this.connect);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.PortNameList);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.Text = "Blaze Firmware Updater";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox PortNameList;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button connect;
        private System.Windows.Forms.Button refresh;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label status;
        private System.Windows.Forms.ProgressBar progress;
        private System.Windows.Forms.Button erase;
        private System.Windows.Forms.Button program;
    }
}

