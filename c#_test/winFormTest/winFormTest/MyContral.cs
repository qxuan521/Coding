using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace winFormTest
{
    public interface  PropertyContral 
    {
        void        build(string szXml);
        void        setValue(string szValue);
        Control     getControl();
    }

    public class IntNumberProperty :   NumericUpDown , PropertyContral
    {
        public IntNumberProperty()
        {
            this.DecimalPlaces = 0;
        }
        public void build(string szXml)
        {
            this.Name = "111111111";
        }
        public void setValue(string szValue)
        {
            int value = 0;
            if(int.TryParse(szValue,out value))
            {
                this.Value = value;
            }
        }
        public Control getControl()
        {
            return this;
        }
    }
    public class FloatNumberProperty : NumericUpDown, PropertyContral
    {
        public FloatNumberProperty()
        {
            this.DecimalPlaces = 2;
        }
        public void build(string szXml)
        {
            this.Name = "222222";
        }
        public void setValue(string szValue)
        {
            float value = 0;
            if (float.TryParse(szValue, out value))
            {
                this.Value = new Decimal( value);
            }
        }
        public Control getControl()
        {
            return this;
        }
    }

    public class StringProperty : TextBox, PropertyContral
    {
        public StringProperty()
        {

        }
        public void build(string szXml)
        {
        }
        public void setValue(string szValue)
        {
            this.Text = szValue;
        }
        public Control getControl()
        {
            return this;
        }
    }
}
