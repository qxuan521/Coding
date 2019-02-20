using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace path_select_contral
{
    public partial class PathSelectControl: UserControl
    {
        System.Windows.Forms.OpenFileDialog m_rWorkTableSelect_dialog = null;
        public PathSelectControl()
        {
            InitializeComponent();
        }

        public void setLabelText(string szText)
        {
            this.label1.Text = szText;
        }

        public void setFileDialog(ref OpenFileDialog rDialog)
        {
            m_rWorkTableSelect_dialog = rDialog;
        }

        public string getPath()
        {
            return this.textBox1.Text;
        }
        //浏览按钮
        private void button1_Click(object sender, EventArgs e)
        {
            if(m_rWorkTableSelect_dialog == null)
            {
                return;
            }
            if (this.m_rWorkTableSelect_dialog.ShowDialog() == DialogResult.OK)
            {
                this.textBox1.Text = m_rWorkTableSelect_dialog.FileName;
            }
        }

        private void textBox1_DragDrop(object sender, DragEventArgs e)
        {
            string path = ((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString();
            this.textBox1.Text = path;
        }

        private void textBox1_DragEnter(object sender, DragEventArgs e)
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
    }
}
