
function main(){
    let __open_2 = Module.getExportByName("libc.so", "__open_2");  // __open_2 是 Android NDK 提供的函數, .cpp裡會調用這個
    let open = Module.getExportByName("libc.so", "open"); // open是標準的C庫函數, .c裡會調用這個

    // let __openat = Module.getExportByName(null, "openat"); 

    Interceptor.attach(__open_2,{
        onEnter:function(args){
            console.log("=====================================")
            console.log("pathname: ",args[0].readCString())



            
        },
        onLeave:function(retVal){
            
        }
    })

}


setImmediate(main)