using EVTechnology.Common.Helper;
using System;

namespace EVClassLib
{
    [Serializable]
    public class SButton : SText
    {
        public SButton()
        {
            this.Type = UIType.BUTTON;
            Event = new BaseEvent();
        }

        public string XMLImagePath { set; get; }

        public override string GetCode()
        {
            string text = "";
            if (!string.IsNullOrEmpty(this.ParentName))
            {
                string tmp;
                if (string.IsNullOrEmpty(Text))
                {
                    tmp = "NULL";
                }
                else
                    tmp = Name + "_text";

                text = String.Format("{0} = CreateControl({1}, {2}, {3}, {4}, {5}, {6}, NULL, {7}, {8}, {9}, 0x{10});\r",
                    Name, Location.X, Location.Y, Size.Width, Size.Height, 0, ParentName, tmp, Text.Length, FontSize, 
                    DrawHelper.RGB32toRGB16(TextColor));
            }
            return text;
        }
    }
}
