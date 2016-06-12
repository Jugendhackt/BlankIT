using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace webui
{
    public partial class Main : System.Web.UI.Page
    {
        protected void HttpRequest(string Text, int Quatschfaktor)
        {
            /* 
             * Senden eines HTTP POST REQUEST mittels HttpWebRequest zu einer Adresse (url).
             */

            string url = "http://192.168.173.28/index.php";
            string fq = "1";
            HttpWebRequest req = (HttpWebRequest)WebRequest.Create(url);
            req.Method = "POST";
            string Data = "text" + "=" + Text + "&fq=" + fq; //SearchBox.Text
            byte[] postBytes = Encoding.ASCII.GetBytes(Data);
            req.ContentType = "application/x-www-form-urlencoded";
            req.ContentLength = Data.Length;
            Stream requestStream = req.GetRequestStream();
            requestStream.Write(postBytes, 0, postBytes.Length);
        }

        protected string HttpResponse()

            HttpWebResponse res = (HttpWebResponse)req.GetResponse();

            Stream responseStream = res.GetResponseStream();
            StreamReader myStreamReader = new StreamReader(responseStream, Encoding.Default);

            string ret = myStreamReader.ReadToEnd();

            myStreamReader.Close();
            responseStream.Close();
            requestStream.Close();
            res.Close();

            ret.Replace(EOF, "");

            //Debug.Text = ret;
            return ret;
        }
    }
}