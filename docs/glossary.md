---
layout: page
title: 'Glossary'
order: 99999999
permalink: /glossary/
---
{% assign glossary_items = site.data.glossary | sort:"term" %}
{% for item in glossary_items %}
**{{ item.term }}**

- {{ item.description }}
{% endfor %}
