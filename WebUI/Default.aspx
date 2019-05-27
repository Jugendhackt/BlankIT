<%@ Page Title="BlankIT" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Default.aspx.cs" Inherits="WebUI.Default" ContentType="Static" %>


<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">
        <div class="jumbotron">
            <p class="lead">
                Gebe einen Text ein, über den du abgefragt werden möchtest:
            </p>
            <p class="lead">
                <asp:TextBox ID="SearchBox" runat="server" Width="90%" Height="92pt" TextMode="MultiLine"/>
                <asp:ImageButton ID="SearchButton" runat="server" ImageUrl="~/images/search.svg" Height="88pt" OnClick="SearchButton_Click" Width="75px"/>
            </p>
            <div class="viewer">
                <p style="width: 100%">
                    <asp:CheckBox ID="WikiEnabled" runat="server" Text="Wikipedia durchsuchen" />
                    <asp:Label ID="Lueckentext" runat="server" Width="100%" Visible="False"></asp:Label><br />
                    <asp:Button ID="SkipQuest" runat="server" OnClick="Button1_Click" Text="Lösung" Visible="False" />
                    <asp:TextBox ID="Lueckenfüller" runat="server" Width="381px" style="margin-left: 317" Visible="False"></asp:TextBox>
                    <asp:ImageButton ID="LueckenButton" runat="server" Height="17px" OnClick="ConfirmInput" OnLoad="LueckenButton_Load" Visible="False" />
                </p>
            </div>
        </div>
    <!--div class="row"-->
        <!--div class="col-md-4"/-->
</asp:Content>
