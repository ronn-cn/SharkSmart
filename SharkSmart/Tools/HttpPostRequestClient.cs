using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Security;
using System.Text;
using System.Threading.Tasks;

namespace SharkSmart
{
    /// <summary>
    /// 用于以 POST 方式向目标地址提交表达数据
    /// 使用 application/x-www-form-urlencoded 编码方式
    /// 不支持上传文件, 若上传文件, 请使用<see cref="HttpPostFileRequestClient"/>
    /// </summary>
    public sealed class HttpPostRequestClient
    {
        #region - Private -
        private List<KeyValuePair<string, string>> _postDatas;
        #endregion

        /// <summary>
        /// 获取或设置数据字符编码, 默认使用<see cref="System.Text.Encoding.UTF8"/>
        /// </summary>
        public Encoding Encoding { get; set; } = Encoding.UTF8;

        /// <summary>
        /// 获取或设置 UserAgent
        /// </summary>
        public string UserAgent { get; set; } = "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/68.0.3440.106 Safari/537.36";

        /// <summary>
        /// 获取或设置 Accept
        /// </summary>
        public string Accept { get; set; } = "*/*";

        /// <summary>
        /// 获取或设置 Referer
        /// </summary>
        public string Referer { get; set; }

        /// <summary>
        /// 获取或设置 Cookie 容器
        /// </summary>
        public CookieContainer CookieContainer { get; set; } = new CookieContainer();

        /// <summary>
        /// 初始化一个用于以 POST 方式向目标地址提交不包含文件表单数据<see cref="HttpPostRequestClient"/>实例
        /// </summary>
        public HttpPostRequestClient()
        {
            this._postDatas = new List<KeyValuePair<string, string>>();
        }

        /// <summary>
        /// 设置表单数据字段, 用于存放文本类型数据
        /// </summary>
        /// <param name="fieldName">指定的字段名称</param>
        /// <param name="fieldValue">指定的字段值</param>
        public void SetField(string fieldName, string fieldValue)
        {
            this._postDatas.Add(new KeyValuePair<string, string>(fieldName, fieldValue));
        }

        /// <summary>
        /// 以POST方式向目标地址提交表单数据
        /// </summary>
        /// <param name="url">目标地址, http(s)://sample.com</param>
        /// <returns>目标地址的响应</returns>
        public HttpWebResponse Post(string url)
        {
            if (string.IsNullOrWhiteSpace(url))
                throw new ArgumentNullException(nameof(url));

            HttpWebRequest request = null;
            if (url.ToLowerInvariant().StartsWith("https"))
            {
                request = WebRequest.Create(url) as HttpWebRequest;
                ServicePointManager.ServerCertificateValidationCallback = new RemoteCertificateValidationCallback((s, c, ch, ss) => { return true; });
                request.ProtocolVersion = HttpVersion.Version11;
                ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls;
                request.KeepAlive = true;
                ServicePointManager.CheckCertificateRevocationList = true; ServicePointManager.DefaultConnectionLimit = 100;
                ServicePointManager.Expect100Continue = false;
            }
            else
            {
                request = WebRequest.Create(url) as HttpWebRequest;
            }


            request.Method = "POST";
            request.ContentType = "application/x-www-form-urlencoded";
            request.UserAgent = this.UserAgent;
            request.Accept = this.Accept;
            request.Referer = this.Referer;
            request.CookieContainer = this.CookieContainer;

            var postData = string.Join("&", this._postDatas.Select(p => $"{p.Key}={p.Value}"));

            using (var requestStream = request.GetRequestStream())
            {
                var bytes = this.Encoding.GetBytes(postData);
                requestStream.Write(bytes, 0, bytes.Length);
            }
            return request.GetResponse() as HttpWebResponse;
        }

        /// <summary>
        /// 以POST方式向目标地址提交表单数据
        /// </summary>
        /// <param name="url">目标地址, http(s)://sample.com</param>
        /// <returns>目标地址的响应</returns>
        public async Task<HttpWebResponse> PostAsync(string url)
        {
            if (string.IsNullOrWhiteSpace(url))
                throw new ArgumentNullException(nameof(url));

            HttpWebRequest request = null;
            if (url.ToLowerInvariant().StartsWith("https"))
            {
                request = WebRequest.Create(url) as HttpWebRequest;
                ServicePointManager.ServerCertificateValidationCallback = new RemoteCertificateValidationCallback((s, c, ch, ss) => { return true; });
                request.ProtocolVersion = HttpVersion.Version11;
                ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls;
                request.KeepAlive = true;
                ServicePointManager.CheckCertificateRevocationList = true; ServicePointManager.DefaultConnectionLimit = 100;
                ServicePointManager.Expect100Continue = false;
            }
            else
            {
                request = WebRequest.Create(url) as HttpWebRequest;
            }

            request.Method = "POST";
            request.ContentType = "application/x-www-form-urlencoded";
            request.UserAgent = this.UserAgent;
            request.Accept = this.Accept;
            request.Referer = this.Referer;
            request.CookieContainer = this.CookieContainer;

            var postData = string.Join("&", this._postDatas.Select(p => $"{p.Key}={p.Value}"));

            using (var requestStream = await request.GetRequestStreamAsync())
            {
                var bytes = this.Encoding.GetBytes(postData);
                requestStream.Write(bytes, 0, bytes.Length);
            }
            return await request.GetResponseAsync() as HttpWebResponse;
        }
    }
}
