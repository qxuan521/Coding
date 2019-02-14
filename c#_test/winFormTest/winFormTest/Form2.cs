using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace winFormTest
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
            this.richTextBox1.ReadOnly = true;
            this.richTextBox1.BackColor = Color.White;
        }

        private void folderBrowserDialog2_HelpRequest(object sender, EventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.openFileDialog1.ShowDialog();
            ExcelOperator rOperator = new ExcelOperator();
            rOperator.ExcelOp("E:\\c#_test\\winFormTest\\winFormTest\\hahaha.xls");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                this.textBox1.Text = folderBrowserDialog1.SelectedPath;
                //设置文本颜色
                richTextBox1.SelectionColor = Color.Red;
                richTextBox1.AppendText(this.textBox1.Text + '\n');
            }
        }

        private void selectDstTable_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                this.textBox2.Text = openFileDialog1.FileName;
                richTextBox1.SelectionColor = Color.Green;
                richTextBox1.AppendText(this.textBox2.Text + '\n');
            }
        }

    }
}
