---
layout: page
title: 'A Primer on Networking'
order: 20
usemathjax: true
---

<div class="ui pointing secondary menu">
  <a class="item active" data-tab="first">Networking</a>
  <a class="item " data-tab="second">Latency & Bandwidth</a>
  <a class="item " data-tab="third">Topologies</a>
  <a class="item " data-tab="fifth">Contention</a>
</div>

<div markdown="1" class="ui tab segment active" data-tab="first">
  {% include_relative include_primer_on_networking/networking.md %}
</div>
<div markdown="1" class="ui tab segment" data-tab="second">
  {% include_relative include_primer_on_networking/latency_and_bandwidth.md %}
</div>
<div markdown="1" class="ui tab segment" data-tab="third">
  {% include_relative include_primer_on_networking/topologies.md %}
</div>
<div markdown="1" class="ui tab segment " data-tab="fifth">
  {% include_relative include_primer_on_networking/contention.md %}
</div>


