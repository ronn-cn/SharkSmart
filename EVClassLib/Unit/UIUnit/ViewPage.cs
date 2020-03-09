using EVTechnology.Common.Helper;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;

namespace EVClassLib
{
    [Serializable]
    public class ViewPage : ITransform
    {
        public string Name { set; get; }

        public UIType Type { get; set; }

        public List<SControl> Controls { set; get; }

        public ViewPage()
        {
            this.Type = UIType.WINDOW;
            this.Controls = new List<SControl>();
        }

        public void SetEvent(BaseEvent et, string CtlName)
        {
            Controls.Find(p => p.Name == Name + "_" + CtlName).Event = et;
        }

        
        public virtual string TransformToClang_forC()
        {
            string defineCode = "EV_PAGE* " + Name + ";\r";
            string unicodeCode = "";
            string eventCode = "";
            string initCode = "";
            foreach (var ctl in Controls)
            {
                if (ctl is SButton)
                {
                    if (!string.IsNullOrEmpty(((SButton)ctl).Text))
                    {
                        unicodeCode += "uint16_t " + ctl.Name + "_text[] = {" +
                            StringHelper.StringToUnicode(((SButton)ctl).Text) + "};\r";
                    }
                    if (!string.IsNullOrEmpty(ctl.Event.Clicked))
                    {
                        eventCode += string.Format(@"
        uint8_t {0}_flag = ContainsPoint({0}, tp_x, tp_y);
        if({0}_flag)
	    {{
		    DisplayUnit_{1}();
	    }}
    ", ctl.Name, ctl.Event.Clicked);
                    }
                }
                initCode += ctl.GetCode();
                if (ctl is SWindow) continue;
                defineCode += "EV_CONTROL* " + ctl.Name + ";\r";
            }

            string text = String.Format(@"
#include ""GLOBAL.h""
//定义
{1}
void {0}_callback()
{{
    //事件
    {2}
}}

//定义字符串
{3}
void Create{0}()
{{
    {4}
}}
", Name, defineCode, eventCode, unicodeCode, initCode);
            return text;
        }

        public string TransformToClang_forH()
        {
            string text = "";
            int i = 1;
            text += "extern EV_PAGE* " + Name + ";\r";
            foreach (SControl ctrl in Controls)
            {
                if (ctrl.Name != this.Name)
                {
                    text += "extern EV_CONTROL* " + ctrl.Name + ";\r\n";
                    if (!string.IsNullOrEmpty(ctrl.Event?.Clicked))
                        text += "extern void DisplayUnit_" + ctrl.Event.Clicked + "();\r";
                    i++;
                }
            }
            text += "extern void Create" + this.Name + "();\r\n";
            return text;
        }
    }
}
