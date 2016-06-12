<%@ Page Title="BlankIT" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Default.aspx.cs" Inherits="WebUI.Default" %>


<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">
        <div class="jumbotron">
            <p class="lead">
                Gebe einen Text ein, über den du abgefragt werden möchtest:
            </p>
            <p class="lead">
                <asp:TextBox ID="SearchBox" runat="server" Width="90%" Height="20pt"/>
                <asp:ImageButton ID="SearchButton" runat="server" ImageUrl="~/images/search.svg" Height="15pt" OnClick="SearchButton_Click"/>
            </p>
            <div class="viewer">
                <p style="width: 100%">
                    <asp:Label ID="Lueckentext" runat="server" Text="" Width="100%"> </asp:Label><br />
                    <asp:TextBox ID="Lueckenfüller" runat="server"></asp:TextBox>
                    <asp:ImageButton ID="LueckenButton" runat="server" />
                </p>
            </div>
        </div>
    <!--div class="row"-->
        <!--div class="col-md-4"/-->
</asp:Content>
