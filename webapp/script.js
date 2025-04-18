let port, reader, writer;
const decoder = new TextDecoderStream();
const output = document.getElementById('data');
const ipListEl = document.getElementById('ipList');
const subnetVisual = document.getElementById('subnetVisual');

document.getElementById('connect').addEventListener('click', async () => {
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
      location.reload();
    });

    await writer.write("ping\n");
    readLoop();
  } catch (err) {
    alert(`Error: ${err.message}`);
  }
});

async function sendCommand(obj) {
  if (writer) {
    await writer.write(JSON.stringify(obj) + '\n');
  }
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
  const subnetMask = `255.255.255.${data.subnets[current]}`;
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
    div.textContent = `#${i}: 192.168.1.${val}`;
    div.onclick = () => sendCommand({ setIndex: i });
    ipListEl.appendChild(div);
  });

  subnetVisual.innerHTML = '';
  for (let i = 0; i < 256; i++) {
    const div = document.createElement('div');
    div.className = 'subnet-block';
    div.textContent = i;

    if (ip.includes(i)) {
      const ipIndex = ip.indexOf(i);
      div.classList.add(`ip-${ipIndex}`);
      if (ipIndex === current) {
        div.classList.add('active');
      }
    }

    if (i === usable[0] - 1 || i === usable[1] + 1) {
      div.classList.add('broadcast');
    } else if (i >= usable[0] && i <= usable[1]) {
      div.classList.add('usable');
    }

    subnetVisual.appendChild(div);
  }
}

