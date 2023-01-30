{
  "targets": [
    { 
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "include_dirs" : [
        "./include",
        "<!@(node -p \"require('node-addon-api').include\")"
        
      ],
      "target_name": "chunk_utils",
      "sources": [
        "./src/bit_array.cc",
        "./src/writer.cc",
        "./src/chunk_utils.cc"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }
  ]
}