using EVTechnology.Common.Controls;
using System;
using System.Collections.Generic;
using System.Drawing;

namespace EVClassLib
{
    [Serializable]
    public class SIconView : SControl
    {
        public Size IconSize { set; get; }

        public Size ItemSize { set; get; }

        public int XSpace { set; get; }

        public int YSpace { set; get; }

        public List<UIIconViewItem> Items { set; get; }

        public SIconView()
        {
            this.Type = UIType.ICONVIEW;
            Event = new ListEvent();
        }

        public override string GetCode()
        {
            string text = "";
            if (!string.IsNullOrEmpty(this.ParentName))
            {
                text += string.Format("	{0} = SHARK_ICONVIEW_CREATE({1}, {2}, {3}, {4}, {5}.Self, WM_CF_SHOW | WM_CF_HASTRANS | 0x{9}{10}, 0, GUI_ID_{6}, {7}, {8});\r\n",
                 Name, Location.X, Location.Y, Size.Width, Size.Height, this.ParentName, Name, ItemSize.Width, ItemSize.Height,
                 Convert.ToString(IconSize.Width, 8), Convert.ToString(IconSize.Height, 8));
                text += string.Format(@"ICONVIEW_SetFrame({0}, GUI_COORD_X, 0);
ICONVIEW_SetFrame({0}, GUI_COORD_X, 0);
ICONVIEW_SetSpace({0}, GUI_COORD_X, {1});
ICONVIEW_SetSpace({0}, GUI_COORD_Y, {2});
ICONVIEW_SetBkColor({0}, ICONVIEW_CI_SEL, SHARK_SKIN_COLOR);
ICONVIEW_SetTextColor({0}, ICONVIEW_CI_UNSEL, GUI_BLACK);
ICONVIEW_SetTextColor({0}, ICONVIEW_CI_SEL, GUI_WHITE);
ICONVIEW_SetFont({0}, &XBF_Font14);
ICONVIEW_SetIconAlign({0}, ICONVIEW_IA_TOP);
ICONVIEW_EnableStreamAuto();
", Name, XSpace, YSpace);

                int i = 0;
                foreach (var it in Items)
                {
                    text += string.Format(@"uint8_t *{0}_{1}_Data;
Load_BMPFileData(""Dta\\{0}_{1}.dta"", &{0}_{1}_Data);
ICONVIEW_AddStreamedBitmapItem({0}, {0}_{1}_Data, ""{2}"");
", Name, i, it.Name);
                    i++;
                }
            }
            return text;
        }
    }
}
