using System;

namespace EVClassLib
{
    [Serializable]
    public class SMultiEdit : SText
    {
        public SMultiEdit()
        {
            this.Type = UIType.MULTIEDIT;
            Event = new ValueEvent();
        }


        public override string GetCode()
        {
            string text = "";
            if (!string.IsNullOrEmpty(this.ParentName))
            {
                text += string.Format("{0} = SHARK_MULTIEDIT_CREATE({1}, {2}, {3}, {4}, {5}, WM_CF_SHOW, 0, GUI_ID_{6}, 200, \"{7}\");\r\n",
                 Name, Location.X, Location.Y, Size.Width, Size.Height, ParentName, Name, ConverString(Text));
                text += string.Format("MULTIEDIT_SetFont({0}, &XBF_Font16);\r\n", Name);
                text += string.Format("MULTIEDIT_SetAutoScrollV({0}, 1);\r\n", Name);
                if (!string.IsNullOrEmpty(Text))
                {
                    text += string.Format("MULTIEDIT_SetText({0}, \"{1}\");\r\n", Name, ConverString(Text));
                }
                text += string.Format("MULTIEDIT_SetTextColor({0}, MULTIEDIT_CI_EDIT, 0x{1});\r\n", Name, ColorToABGR(TextColor));
                text += string.Format("MULTIEDIT_SetTextAlign({0}, {1});\r\n", Name, GetAlignCode());
                text += string.Format("MULTIEDIT_SetFont({0}, &XBF_Font{1});\r", Name, FontSize);
            }
            return text;
        }
    }
}
