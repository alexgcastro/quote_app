{
  "targets": [
    {
      "target_name": "text_rendering",
      "sources": [ "text_rendering_addon/TemplateUtils.h",
                   "text_rendering_addon/TemplateUtils.cc",
                   "text_rendering_addon/Template.h",
                   "text_rendering_addon/TemplateBlueX.h",
                   "text_rendering_addon/TemplateBlueX.cc",
                   "text_rendering_addon/text_rendering.cc"],
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
