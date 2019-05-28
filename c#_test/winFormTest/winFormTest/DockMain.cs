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
    public partial class DockMain : Form
    {
        DynamicTest m_rL = new DynamicTest();

        public DockMain()
        {
            InitializeComponent();
        }
        private void DockMain_Load(object sender, EventArgs e)
        {

            m_rL.TopLevel = false;
            this.panel1.Controls.Add(m_rL);
            m_rL.Dock = DockStyle.Fill;
            m_rL.Show();
            //m_rL.Dock = DockStyle.Right;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            m_rL.updateView("LUT");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            m_rL.updateView("SMAA");
        }

        private void button3_Click(object sender, EventArgs e)
        {

        }

        private void button4_Click(object sender, EventArgs e)
        {

        }
    }
}
