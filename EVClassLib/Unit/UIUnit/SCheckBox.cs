using System;

namespace EVClassLib
{
    [Serializable]
    public class SCheckBox : SText
    {
        public bool IsSwitch { set; get; }
        public SCheckBox()
        {
            this.Type = UIType.CHECKBOX;
            Event = new ValueEvent();
        }

        public override string GetCode()
        {
            string text = "";
            if (!string.IsNullOrEmpty(this.ParentName))
            {
                text += String.Format("{0} = SHARK_CHECKBOX_CREATE({1}, {2}, {3}, {4}, {5}, WM_CF_SHOW, 0, GUI_ID_{6}, \"{7}\");\r\n",
                    this.Name, this.Location.X, this.Location.Y, this.Size.Width, this.Size.Height, this.ParentName, this.Name, this.Text);
                text += string.Format("CHECKBOX_SetFont({0}, &XBF_Font{1});\r", Name, FontSize);
                if (IsSwitch)
                {
                    text += string.Format("CHECKBOX_SetSkin({0}, SHARK_CHECKBOX_SKIN_SWITCH);\r\n", this.Name);
                }
            }
            return text;
        }
    }
}
