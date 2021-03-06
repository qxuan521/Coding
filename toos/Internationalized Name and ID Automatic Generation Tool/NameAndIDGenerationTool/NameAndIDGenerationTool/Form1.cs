﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace NameAndIDGenerationTool
{
    public partial class Form1 : Form
    {
        DataResolve m_rDataResolve;
        public Form1()
        {
            InitializeComponent();
            m_rDataResolve = new DataResolve();
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            string szSrcPath = m_rDataResolve.getSrcPath();
            string szResultPaht = m_rDataResolve.getResultPath();
            if (szSrcPath != "" && szSrcPath != null)
            {
                this.m_rSrcTextBlock.Text = szSrcPath;
                this.m_rDataResolve.initialize(szSrcPath, ref this.richTextBox1);
                this.m_rDataResolve.testFunc(ref this.richTextBox1);
            }
            if (szResultPaht != "" && szResultPaht != null)
            {
                this.m_rOutPutTextBlock.Text = szResultPaht;
            }
        }

        private void m_rSrcFolderSelect_btn_Click(object sender, EventArgs e)
        {
            if (this.m_rSrcTableSelect_dialog.ShowDialog() == DialogResult.OK)
            {
                string szPath = m_rSrcTableSelect_dialog.SelectedPath;
                this.m_rSrcTextBlock.Text = szPath;
                this.m_rDataResolve.initialize(szPath, ref this.richTextBox1);
                this.m_rDataResolve.testFunc(ref this.richTextBox1);
            }
        }

        private void m_rFillIn_btn_Click(object sender, EventArgs e)
        {
            this.m_rDataResolve.completionData(m_rWorkTableTextBlock.Text, this.m_rOutPutTextBlock.Text, ref this.richTextBox1);
        }

        private void m_rWorkTableSelect_btn_Click(object sender, EventArgs e)
        {
            if (this.m_rWorkTableSelect_dialog.ShowDialog() == DialogResult.OK)
            {
                this.m_rWorkTableTextBlock.Text = m_rWorkTableSelect_dialog.FileName;
            }
        }

        private void m_rResultFolderSelect_btn_Click(object sender, EventArgs e)
        {
            if (this.m_rResultFolder.ShowDialog() == DialogResult.OK)
            {
                this.m_rOutPutTextBlock.Text = this.m_rResultFolder.SelectedPath;
            }
        }

        private void m_rCheck_btn_Click(object sender, EventArgs e)
        {
            this.m_rDataResolve.checkData(m_rWorkTableTextBlock.Text, ref this.richTextBox1);
        }

        private void m_rWorkTableTextBlock_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.Link;
            } 
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }

        private void m_rWorkTableTextBlock_DragDrop(object sender, DragEventArgs e)
        {
            string path = ((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString();
            m_rWorkTableTextBlock.Text = path;
        }
    }
}
