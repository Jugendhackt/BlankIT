using System;
using System.Threading.Tasks;
using Microsoft.Owin;
using Owin;

[assembly: OwinStartup(typeof(WebUI2.Startup))]

namespace WebUI2
{
    public class Startup
    {
        public void Configuration(IAppBuilder app)
        {
            // Weitere Informationen zm Konfigurieren Ihrer Anwendung finden Sie unter "http://go.microsoft.com/fwlink/?LinkID=316888".
        }
    }
}
