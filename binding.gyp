{
  "targets": [
    {
      "target_name": "text_rendering",
      "sources": [ "text_rendering_addon/text_rendering.cc" ],
      'cflags': [
        '<!@(pkg-config --cflags pangocairo)',
      ],
      'link_settings': {
        'libraries': [
          '<!@(pkg-config --libs-only-l pangocairo)',
        ]
      }
    }
  ]
}
