using System;
using System.Web.UI;
using System.Net;
using System.Text;
using System.IO;

namespace autoquiz
{
    public partial class Default : Page
    {

        protected void SearchButton_Click(object sender, EventArgs e)
        {
            try
            {
                string url = "http://192.168.173.28/index.php";
                string fq = "2";
                HttpWebRequest req = (HttpWebRequest)WebRequest.Create(url);
                req.Method = "POST";
                string Data = "text=" + SearchBox.Text + "\n fq=" + fq;
                byte[] postBytes = Encoding.ASCII.GetBytes(Data);
                req.ContentType = "application/x-www-form-urlencoded";
                req.ContentLength = Data.Length;
                Stream requestStream = req.GetRequestStream();
                requestStream.Write(postBytes, 0, postBytes.Length);
                requestStream.Close();

                HttpWebResponse response = (HttpWebResponse)req.GetResponse();
                Stream resStream = response.GetResponseStream();

                var sr = new StreamReader(response.GetResponseStream());
                string responseText = sr.ReadToEnd();
            }
            catch (WebException)
            {
            }
        }
    }
}