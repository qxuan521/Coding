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
    public partial class grid : Form
    {
        public grid()
        {
            InitializeComponent();
        }

        private void grid_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                string szPath = openFileDialog1.FileName;
                this.textBox1.Text = szPath;
                Dictionary<string, exchangeType> rResult = new Dictionary<string, exchangeType>();
                xmlOperator.readXmlToGrid(szPath, ref rResult);
                this.dataGridView1.DataSource = new BindingSource(rResult, null);
            }
        }

        private void dataGridView1_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {
            if (e.ColumnIndex >= 0 && e.RowIndex >= 0)
            {
                if (this.dataGridView1.Columns[e.ColumnIndex].HeaderText == "操作")
                {
                    StringFormat sf = StringFormat.GenericDefault.Clone() as StringFormat;//设置重绘入单元格的字体样式
                    sf.FormatFlags = StringFormatFlags.DisplayFormatControl;
                    sf.Alignment = StringAlignment.Center;
                    sf.LineAlignment = StringAlignment.Center;
                    sf.Trimming = StringTrimming.EllipsisCharacter;

                    e.PaintBackground(e.CellBounds, false);//重绘边框

                    //设置要写入字体的大小
                    System.Drawing.Font myFont = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
                    SizeF sizeDel = e.Graphics.MeasureString("删除", myFont);
                    SizeF sizeMod = e.Graphics.MeasureString("修改", myFont);
                    SizeF sizeLook = e.Graphics.MeasureString("查看", myFont);

                    float fDel = sizeDel.Width / (sizeDel.Width + sizeMod.Width + sizeLook.Width); //
                    float fMod = sizeMod.Width / (sizeDel.Width + sizeMod.Width + sizeLook.Width);
                    float fLook = sizeLook.Width / (sizeDel.Width + sizeMod.Width + sizeLook.Width);

                    //设置每个“按钮的边界”
                    RectangleF rectDel = new RectangleF(e.CellBounds.Left, e.CellBounds.Top, e.CellBounds.Width * fDel, e.CellBounds.Height);
                    RectangleF rectMod = new RectangleF(rectDel.Right, e.CellBounds.Top, e.CellBounds.Width * fMod, e.CellBounds.Height);
                    RectangleF rectLook = new RectangleF(rectMod.Right, e.CellBounds.Top, e.CellBounds.Width * fLook, e.CellBounds.Height);
                    e.Graphics.DrawString("删除", myFont, Brushes.Black, rectDel, sf); //绘制“按钮”
                    e.Graphics.DrawString("修改", myFont, Brushes.Black, rectMod, sf);
                    e.Graphics.DrawString("查看", myFont, Brushes.Black, rectLook, sf);
                    e.Handled = true;
                }
            }
        }

        private void dataGridView1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.ColumnIndex >= 0 && e.RowIndex >= 0)
            {
                Point curPosition = e.Location;//当前鼠标在当前单元格中的坐标
                if (this.dataGridView1.Columns[e.ColumnIndex].HeaderText == "操作")
                {
                    Graphics g = this.dataGridView1.CreateGraphics();
                    System.Drawing.Font myFont = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
                    SizeF sizeDel = g.MeasureString("删除", myFont);
                    SizeF sizeMod = g.MeasureString("修改", myFont);
                    SizeF sizeLook = g.MeasureString("查看", myFont);
                    float fDel = sizeDel.Width / (sizeDel.Width + sizeMod.Width + sizeLook.Width);
                    float fMod = sizeMod.Width / (sizeDel.Width + sizeMod.Width + sizeLook.Width);
                    float fLook = sizeLook.Width / (sizeDel.Width + sizeMod.Width + sizeLook.Width);

                    Rectangle rectTotal = new Rectangle(0, 0, this.dataGridView1.Columns[e.ColumnIndex].Width, this.dataGridView1.Rows[e.RowIndex].Height);
                    RectangleF rectDel = new RectangleF(rectTotal.Left, rectTotal.Top, rectTotal.Width * fDel, rectTotal.Height);
                    RectangleF rectMod = new RectangleF(rectDel.Right, rectTotal.Top, rectTotal.Width * fMod, rectTotal.Height);
                    RectangleF rectLook = new RectangleF(rectMod.Right, rectTotal.Top, rectTotal.Width * fLook, rectTotal.Height);
                    //判断当前鼠标在哪个“按钮”范围内
                    if (rectDel.Contains(curPosition))//删除
                    {
                        MessageBox.Show("1");
                    }
                    else if (rectMod.Contains(curPosition))//修改
                        MessageBox.Show("2");
                    else if (rectLook.Contains(curPosition))//查看
                        MessageBox.Show("3");
                }
            }
        }
    }
}
