<h1>NesTuner</h1>
<p>Fully designed and prototyped sound tuner built using the Raspberry Pi Pi, an electret microphone, and an I2C oled display. The system performs analog signal conditioning, pitch detection, and real-time note estimation using C without external DSP libraries.</p>
<h2>Overview</h2> 
<p>This project captures audio from a microphone, filters a range of frequencies and amplifies the signal using a custom analog hardware design, and feeds it into the Pico's ADC. Using logarithmic frequency mapping and zero-crossing pitch detection, the algorithm I designed determines the nearest musical note and its cents, and it displays the tuning direction all on the OLED display.</p>

<p>This project was designed entirely from scratch using LTSpice analog circuit simulation/development, breadboard prototyping, and C to develop low-level I2C drivers for the oled display.</p>

<h2>Features</h2>
<ul>
  <li>Real-time pitch mapping/tracking (80 Hz - 35000 Hz) </li>
  <li>Tuning error in cents (+- 100)</li>
  <li>Tuning direction indicators (< | >) </li>
  <li>Auto calibation DC offset removal from the mic's input</li>
  <li>Fully offline capabilities</li>
</ul>

<h2>Design Process</h2>
<ol>
  <li>LTSpice design/simulation</li>
    <ul>
      <li>Electret Microphone</li>
        <ul>
          <li>First, I modeled the application circuit in the CMC-5042PF-AC as closely as possible, however, I had to use an AC test source (SINE(0 1m 440)) since SPICE can't directly simulate the microphone. I used an as accurate as possible AC signal to what the microphone would output before amplification with a 1mV amplitude signal and a constant 440 Hz frequency which is recognized as the standard pitch for tuning musical instruments. </li>
          <li>Both the bias resistor (2.2k) for a 3.3V voltage source and the coupling capacitor (1mu) were included in the design. The bias resistor powers the internal JFET of the electret microphone and sets the correct operating current, creating a DC-biased output that carries the AC audio signal. This AC signal then requires a coupling capacitor in order to block the DC component before signal amplifcation. </li>
        </ul>
      <li>Amplifcation</li>
        <ul>
          <li>I then created a typical noninverting amplifier circuit with a biased input and reference voltage of 1.65V using the MCP6002 Low-Power Op Amp. I calculated the gain using basic circuit analysis to find the formula (Gain = 1 + R1/R2). Based on my input voltage I decided a 90-110x amplifcation or about 40dB would be necessary for proper post-signal analysis, so I chose resistance values of 1k and 100k. </li>
        </ul>
      <li>Filtering</li>
        <ul>
          <li>In order to reduce noise I then passed the amplified signal through a simple RC low pass filter. If I were to sample at a rate of 8kHz this means the Nyquist frequency would be 8khz/2 or 4kHz, so I chose a cutoff frequency slightly below this to prevent any aliasing at about 3.5kHz. Through circuit analysis, a single-pole RC LPF cutoff frequency can be calculated using the formula f=1/(2*pi*R*C), so I chose a 4.7nF capacitor and a 10k resistor for an approximate 3.4kHz cutoff.  </li>
          <li>Similarily, I created a high pass filter reversing the resistor and capacitor order targeting a cutoff frequency of 80hZ which led to values of 20nF for the capactior and 100k for the resistor which was also biased at 1.65V. I did notice that using two cascaded filters resulted in my amplifcation being reduced to about 10x or 19dB, however, not filtering out these frequencies created large noise in the adc values from the internals of the circuit. </li>
        </ul>
      <li>Voltage Divider</li>
        <ul>
          <li>In order for the op amp to handle signals below 0V, I had to create a mid-rail bias of 1.65V by shifting the signal up. This was accomplished simply using a voltage divider connected to two capacitors to create a stable bias which would be unaffected by the AC signal. I simply chose values which would best fit my needs in this case which were 10mu and 100n farads. </li>
        </ul>
    </ul>
  <li>Breadboard Prototyping/Software</li> 
    <ul>
      <li> After verifying each stage of my circuit through simulation, I started building the actual hardware. First, I had to verify the I2C display would function correctly according to my needs, so I created a circuit solely to debug the display using the raspberry pi pico's I2C1 (SDA and SCL) pins. I then created the project's core layout using a standard Cmake skeleton, making sure to include the libraries for the i2c display from the pico SDK. From here, I created my own low-levl drivers for the display as well as several API function's which would be of use for UI purposes.</li>
      <li> I then wired each component according to my final SPICE design with the oled display connected as well. I made sure to debug each stage as well using a oscilloscope and multimeter to account for any differences from the ideal design. Once I verified each stage fully, I connected the output of the circuit to the ADC pin of the Pico to gather raw data and debug. At this point, a lot of debugging was necessary, so I had to use the serial monitor from my computer to verify the raw ADC values as well as fine tune my resistor and capacitor values.</li>
      <li>With the raw ADC values, I had to create an algorithm which would convert this raw data into ..(sampling rate 8khz or 125mus | signal processing).. I then finally used the API I had created for the display to create a UI which would display the musical note, tuning error, and tuning direction.</li>
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
      <td>Electret Condenser Microphone (CMC-5042PF-AC)</td>
        <td> 1 </td>
        <td><a href="https://www.digikey.com/en/products/detail/same-sky-formerly-cui-devices/CMC-5042PF-AC/1869984">Digikey</a></td>
    </tr>
    <tr>
      <td>Resistors (0.25 W)</td>
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
    <tr>
      <td>Soldering Iron/Multimeter/Oscilloscope</td>
        <td> Variety </td>
        <td><a>Amazon/Digikey/etc</a></td>
    </tr>
  </tbody>
</table>

<h2>Future Improvements</h2>
<p>In the future, I hope to improve noise performance and gain control, as the current design occasionally amplifies background hiss and reduces signal clarity. Pitch accuracy also requires refinement, and slight deviations are present when input levels fluctuate or harmonics interfere with detection. In addition, the user interface can be improved upon for clearer/more user friendly and faster feedback from the display. Finally, migrating to a custom PCB and enclosure will be able to reduce wire noise which will likely make the finaly producut much more accurate.
