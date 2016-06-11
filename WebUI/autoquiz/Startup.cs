using Microsoft.Owin;
using Owin;

[assembly: OwinStartupAttribute(typeof(autoquiz.Startup))]
namespace autoquiz
{
    public partial class Startup {
        public void Configuration(IAppBuilder app) {
            ConfigureAuth(app);
        }
    }
}
