let port, reader, writer;
const decoder = new TextDecoderStream();
const output = document.getElementById('data');
const ipListEl = document.getElementById('ipList');
const subnetVisual = document.getElementById('subnetVisual');
const connectBtn = document.getElementById('connect');

connectBtn.addEventListener('click', async () => {
  if (port) {
    await port.close();
    port = null;
    connectBtn.textContent = "Connect to ESP32";
    return;
  }

  try {
    port = await navigator.serial.requestPort();
    await port.open({ baudRate: 115200 });

    const encoder = new TextEncoderStream();
    encoder.readable.pipeTo(port.writable);
    writer = encoder.writable.getWriter();

    port.readable.pipeTo(decoder.writable);
    reader = decoder.readable.getReader();

    port.addEventListener("disconnect", () => {
      alert("Device disconnected.");
      connectBtn.textContent = "Connect to ESP32";
      port = null;
      location.reload();
    });

    await writer.write("ping\n");
    connectBtn.textContent = "Disconnect";
    readLoop();
  } catch (err) {
    alert(`Error: ${err.message}`);
  }
});

function sendCommand(obj) {
  if (writer) {
    writer.write(JSON.stringify(obj) + '\n');
  }
}

function isValidSubnet(value) {
  const valid = [0, 128, 192, 224, 240, 248, 252];
  return valid.includes(value);
}

function validateAndSendSubnet() {
  const subnetInput = document.getElementById("newSubnet");
  const errorMsg = document.getElementById("subnetError");
  const value = parseInt(subnetInput.value);

  if (isNaN(value) || !isValidSubnet(value)) {
    errorMsg.style.display = "block";
    return;
  }

  errorMsg.style.display = "none";
  sendCommand({ setSubnet: value });
}

async function readLoop() {
  let buffer = '';
  try {
    while (true) {
      const { value, done } = await reader.read();
      if (done) break;
      buffer += value;

      let lines = buffer.split('\n');
      buffer = lines.pop();

      for (const line of lines) {
        const trimmed = line.trim();
        if (!trimmed.startsWith('{') || !trimmed.endsWith('}')) continue;
        try {
          const json = JSON.parse(trimmed);
          displayData(json);
        } catch (err) {
          console.warn("Parse failed:", trimmed);
        }
      }
    }
  } catch (err) {
    console.error("Serial read error:", err);
    alert("Serial device disconnected unexpectedly.");
  }
}

function displayData(data) {
  const { subnet, cidr, usable, hosts, current, ip } = data;
  const subnetMask = `255.255.255.${subnet}`;
  const ipString = `192.168.1.${ip[current]}`;
  const usableRange = `192.168.1.${usable[0]} - 192.168.1.${usable[1]}`;

  const grid = output.querySelectorAll('div');
  grid[1].textContent = ipString + ' /' + cidr;
  grid[3].textContent = subnetMask;
  grid[5].textContent = usableRange;
  grid[7].textContent = hosts;

  ipListEl.innerHTML = '';
  ip.forEach((val, i) => {
    const div = document.createElement('div');
    div.className = 'ip-entry' + (i === current ? ' selected' : '');
    div.textContent = `#${i}: 192.168.1.${val} /${countBits(data.ip[i], data.subnet)}`;
    div.onclick = () => sendCommand({ setIndex: i });
    ipListEl.appendChild(div);
  });

  subnetVisual.innerHTML = '';
  for (let i = 0; i < 256; i++) {
    const div = document.createElement('div');
    div.className = 'subnet-block';
    div.textContent = i;
  
    // Show all 4 IPs with separate colors
    ip.forEach((val, idx) => {
      if (i === val) div.classList.add(`ip-${idx}`);
    });
  
    // Mark current IP
    if (i === ip[current]) div.classList.add('active');
  
    if (i >= usable[0] && i <= usable[1]) {
      div.classList.add('usable');
    } else if (i === usable[0] - 1 || i === usable[1] + 1) {
      div.classList.add('broadcast');
    }
  
    subnetVisual.appendChild(div);
  }
  
}

function countBits(ip, subnet) {
  let bits = 24;
  let b = subnet;
  while (b) {
    bits += b & 1;
    b >>= 1;
  }
  return bits;
}