//Make sure the page has fully loaded
$(document).ready(function(){
  $("#chat-title").click(function(){
    $("#chat-content").slideToggle("50","easeOutExpo");
  });
         
  $("#chat-user .icon-remove").click(function(){ 
    $("#chat-message").hide();
    $("#chat-contacts").css("height","379px");
  });
              
  $("#chat-contacts .contact").each(function(n){
    $(this).bind("click",function(){
      var name = $(this).data("name");
      $("#chat-dialog").attr('conversation', $(this).data("id"));
      $("#chat-contacts").css("height","200px"); 
      $("#chat-message").show();  
      $("#chat-user .name").text(name);
      $("#chat-dialog").scrollTop($("#chat-dialog")[0].scrollHeight);
    });
  });
  $("#chat-dialog").scrollTop($("#chat-dialog")[0].scrollHeight);

  $(window).scroll(function(){
    if ($(window).scrollTop() >= "94"){
      $("#chat").css({
        "position" : "fixed",
        "top" : "15px",
        "width" : "238px"
      });
    }
    if ($(window).scrollTop() < "94"){
      $("#chat").css({
        "position" : "relative",
        "top" : "0px",
        "width" : "238px"
      });
     }
  });

	
  /*
	* To avoid using buttons, we can just user ENTER to send the message
	*/
  $("#chat-data").keypress(function(e){
    //If the pressed key is ENTER, then we can send the message
    if(e.which == 13) {
      //We also grab the message from "chat-msg". If its empty, we return false, stopping the empty message from being sent
      var msg = $("#chat-data").val() !== "" ? $("#chat-data").val() : false;
      var userChat = $("#chat-data").data('user') !== "" ? $("#chat-data").data('user') : false;
      //It chat-msg is not empty, we send the contents
      if(msg) {
        /*
				* To prevent cache from server, we just append a random string at the end of our query string using javascript Math.random()
				*/
        msg=msg.replace("&","&#38;");
        msg=msg.replace("+","&#43;");
        var url = "talk.action.php?msg="+encodeURIComponent(msg)+"&id="+Math.random()+"&userChat="+userChat+"&conversation="+$("#chat-dialog").attr('conversation');
        //Send message
        $.get(url, function(data){
          //Empty chat-msg
          $("#chat-data").val("");
          $("#chat-dialog").attr({
            scrollTop: $("#chat-dialog").attr("scrollHeight")
          });
          $("#chat-dialog").animate({
            scrollTop: $("#chat-dialog").attr("scrollHeight")
          }, 2000); 
        });
      }
      //Here we just stop the default action of ENTER, which means its now going to only send the message and not insert new-line
      e.preventDefault();
    }
  });
	
  //this is the function we are going to user to load messages at a regular interval
  function loadChat() {
    // we only retrieve new messages else nothing by sending a check-kounter first
    if ($("#chat-dialog").attr('conversation') != 0) {
      var count = $("#chat-count").text();
        $.get("talk.action.php", {
        conversation: $("#chat-dialog").attr('conversation'), 
        count: count
      },
      function(data){
        $("#chat-count").text(data);
        if(count != data) {
          $("#chat-dialog").load("talk.action.php?conversation="+$("#chat-dialog").attr('conversation')+"&k="+count);
          $("#chat-dialog").scrollTop($("#chat-dialog")[0].scrollHeight);
        }
      });
    }
  }
	
  //here we will be retrieving messages every 1 second
  window.setInterval(function(){
    loadChat()
  },1000);

});
            
function heighty () {
  $('.lazyHeight').each(function(n) { 
    var documentHeight = $(document).height();
    $(this).css("height", documentHeight + "px");
  });
}
          