namespace X51Tools.TopicPictureDemandGenerationTool
{
    partial class TableCoverStrInput
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
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.m_rNameBlock = new System.Windows.Forms.TextBox();
            this.m_rIDBlock = new System.Windows.Forms.TextBox();
            this.m_rCommitBtn = new System.Windows.Forms.Button();
            this.m_rRejectBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "套装名称：";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 87);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(77, 12);
            this.label2.TabIndex = 1;
            this.label2.Text = "图鉴贴图ID：";
            // 
            // m_rNameBlock
            // 
            this.m_rNameBlock.BackColor = System.Drawing.Color.White;
            this.m_rNameBlock.Location = new System.Drawing.Point(15, 43);
            this.m_rNameBlock.Name = "m_rNameBlock";
            this.m_rNameBlock.ReadOnly = true;
            this.m_rNameBlock.Size = new System.Drawing.Size(358, 21);
            this.m_rNameBlock.TabIndex = 2;
            // 
            // m_rIDBlock
            // 
            this.m_rIDBlock.Location = new System.Drawing.Point(16, 121);
            this.m_rIDBlock.Name = "m_rIDBlock";
            this.m_rIDBlock.Size = new System.Drawing.Size(358, 21);
            this.m_rIDBlock.TabIndex = 3;
            this.m_rIDBlock.TextChanged += new System.EventHandler(this.m_rIDBlock_TextChanged);
            // 
            // m_rCommitBtn
            // 
            this.m_rCommitBtn.Location = new System.Drawing.Point(43, 168);
            this.m_rCommitBtn.Name = "m_rCommitBtn";
            this.m_rCommitBtn.Size = new System.Drawing.Size(100, 25);
            this.m_rCommitBtn.TabIndex = 4;
            this.m_rCommitBtn.Text = "确   定";
            this.m_rCommitBtn.UseVisualStyleBackColor = true;
            this.m_rCommitBtn.Click += new System.EventHandler(this.m_rCommitBtn_Click);
            // 
            // m_rRejectBtn
            // 
            this.m_rRejectBtn.Location = new System.Drawing.Point(240, 168);
            this.m_rRejectBtn.Name = "m_rRejectBtn";
            this.m_rRejectBtn.Size = new System.Drawing.Size(100, 25);
            this.m_rRejectBtn.TabIndex = 5;
            this.m_rRejectBtn.Text = "取   消";
            this.m_rRejectBtn.UseVisualStyleBackColor = true;
            this.m_rRejectBtn.Click += new System.EventHandler(this.m_rRejectBtn_Click);
            // 
            // TableCoverStrInput
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(390, 215);
            this.Controls.Add(this.m_rRejectBtn);
            this.Controls.Add(this.m_rCommitBtn);
            this.Controls.Add(this.m_rIDBlock);
            this.Controls.Add(this.m_rNameBlock);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "TableCoverStrInput";
            this.Text = "图鉴切图ID输入";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox m_rNameBlock;
        private System.Windows.Forms.TextBox m_rIDBlock;
        private System.Windows.Forms.Button m_rCommitBtn;
        private System.Windows.Forms.Button m_rRejectBtn;
    }
}