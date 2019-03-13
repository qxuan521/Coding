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
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Form2 r = new Form2();
            r.Show();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Form3 r = new Form3();
            r.Show();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            grid r = new grid();
            r.Show();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            DockMain rTestForm = new DockMain();
            rTestForm.Show();
        }
    }
}
