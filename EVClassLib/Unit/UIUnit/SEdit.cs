using System;

namespace EVClassLib
{
    [Serializable]
    public class SEdit : SText
    {
        public SEdit()
        {
            this.Type = UIType.EDIT;
            Event = new ValueEvent();
        }

        public override string GetCode()
        {
            string text = "";
            if (!string.IsNullOrEmpty(this.ParentName))
            {
                text += String.Format("{0} = SHARK_EDIT_CREATE({1}, {2}, {3}, {4}, {5}, WM_CF_SHOW, 0, GUI_ID_{6}, 50);\r\n",
                    this.Name, this.Location.X, this.Location.Y, this.Size.Width, this.Size.Height, this.ParentName, this.Name);

                if (!string.IsNullOrEmpty(Text))
                {
                    text += string.Format("EDIT_SetText({0}, \"{1}\");\r\n", Name, ConverString(Text));
                }
                text += string.Format("EDIT_SetTextColor({0}, EDIT_CI_ENABLED, 0x{1});\r\n", Name, ColorToABGR(TextColor));
                text += string.Format("EDIT_SetTextAlign({0}, {1});\r\n", Name, GetAlignCode());
                text += string.Format("EDIT_SetFont({0}, &XBF_Font{1});\r", Name, FontSize);
            }
            return text;
        }
    }
}
