using System;

namespace EVClassLib
{
    [Serializable]
    public class SHeader : SText
    {
        public SHeader()
        {
            this.Type = UIType.HEADER;
            Event = new ValueEvent();
        }

        public override string GetCode()
        {
            string text = "";
            if (!string.IsNullOrEmpty(this.ParentName))
            {
                text += String.Format("{0} = SHARK_HEADER_CREATE({1}, {2}, {3}, {4}, {5}, WM_CF_SHOW, 0, GUI_ID_{6});\r\n",
                    this.Name, this.Location.X, this.Location.Y, this.Size.Width, this.Size.Height, this.ParentName, this.Name);
                text += string.Format("HEADER_SetTextColor({0}, 0x{1});\r\n", Name, ColorToABGR(TextColor));
                text += string.Format("HEADER_AddItem({0}, {1}, \"{2}\", GUI_TA_HCENTER|GUI_TA_VCENTER);\r\n", Name, Size.Width, ConverString(Text));
                text += string.Format("HEADER_SetFont({0}, &XBF_Font{1});\r", Name, FontSize);
            }
            return text;
        }
    }
}
