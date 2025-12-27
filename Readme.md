<h1>NesTuner</h1>
<p>Fully designed and prototyped sound tuner built using the Raspberry Pi Pi, an electret microphone, and an I2C oled display. The system performs analog signal conditioning, pitch detection, and real-time note estimation using C without external DSP libraries.</p>
<h2>Overview</h2> 
<p>This project captures audio from a microphone, filters a range of frequencies and amplifies the signal using a custom analog hardware design, and feeds it into the Pico's ADC. Using logarithmic frequency mapping and zero-crossing pitch detection, the algorithm I designed determines the nearest musical note and its cents, and it displays the tuning direction all on the OLED display.</p>

<p>This project was designed entirely from scratch using LTSpice analog circuit simulation/development, breadboard prototyping, and C to develop low-level I2C drivers for the oled display.</p>

<h2>Features</h2>
<ul>
  <li>Real-time pitch mapping/tracking (50 Hz - 1000 Hz) </li>
  <li>Tuning error in cents (+- 100)</li>
  <li>Tuning direction indicators (< | >) </li>
  <li>Auto calibation DC offset removal from the mic's input</li>
  <li>Fully offline capabilities</li>
</ul>

<h2>Design Process</h2>
<ol>
  <li>LTSpice simulation</li>
    <ul>
      <li> .....</li>
    </ul>
  <li>Breadboard Prototyping</li>
    <ul>
      <li> ....</li>
    </ul>
  <li>Software</li>
    <ul>
      <li>......</li>
    </ul>
</ol>

<h2>Bill of Materials</h2>
<table>
  <thead>
    <tr>
      <th>Component</th>
      <th>Quantity</th>
      <th>Market</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Raspberry Pi Pico (RP2040) w/ USB</td>
        <td> 1 </td>
        <td><a href="https://www.digikey.com/en/products/detail/raspberry-pi/SC0915/13684020">Digikey</a></td>
    </tr>
    <tr>
      <td>SSD1306 Oled I2C display</td>
        <td> 1 </td>
        <td><a href="https://www.amazon.com/dp/B09T6SJBV5?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1">Amazon</a></td>
    </tr>
    <tr>
      <td>MCP6002 Opamp</td>
        <td> 1 </td>
        <td><a href="https://www.digikey.com/en/products/detail/microchip-technology/MCP6002-I-P/500875">Digikey</a></td>
    </tr>
    <tr>
      <td>Electret Microphone</td>
        <td> 1 </td>
        <td><a href="https://www.digikey.com/en/products/detail/same-sky-formerly-cui-devices/CMC-5042PF-AC/1869984">Digikey</a></td>
    </tr>
    <tr>
      <td>Resistors</td>
        <td> Variety </td>
        <td><a>Amazon/Digikey/etc</a></td>
    </tr>
    <tr>
      <td>Capactiors (electrolytic/ceramic)</td>
        <td> Variety </td>
        <td><a>Amazon/Digikey/etc</a></td>
    </tr>
    <tr>
      <td>Breadboard/Wires</td>
        <td> Variety </td>
        <td><a>Amazon/Digikey/etc</a></td>
    </tr>
  </tbody>
</table>

<h2>Future Improvements</h2>
<ul>
  <li>Noise Handling / Gain Control </li>
  <li>Pitch Accuracy</li>
  <li>UI Improvement</li>
  <li>PCB + Enclosure</li>
</ul>
