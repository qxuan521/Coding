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
    public partial class Form3 : Form
    {
        public Form3()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            xmlOperator.CreateXML("clothset_handbook_config.xml", ref this.textBox1);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            xmlOperator.outPutNewFile("clothset_handbook_config.xml");
        }
    }
}
