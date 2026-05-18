interface SystemStats
{
    cpuUsage: number;
    memUsage: number;
    memTotal: number;
    memUsed: number;
}

class ResourceMonitor
{
    private readonly maxDash: number = 126; // The length of the full arc in SVG

    public async tick(): Promise<void>
    {
        try
        {
            const response = await fetch("/api/stats");
            const data: SystemStats = await response.json();

            this.updateElement("cpu", data.cpuUsage);
            this.updateElement("mem", data.memUsage);

            const memDetails = document.getElementById("mem-abs");
            if (memDetails)
            {
                memDetails.innerText = `${data.memUsed.toFixed(1)} / ${data.memTotal.toFixed(1)} GB`;
            }
        }
        catch (err)
        {
            console.error("Connection error");
        }
    }

    private updateElement(id: string, value: number): void
    {
        const textElem = document.getElementById(`${id}-text`);
        const gaugeElem = document.getElementById(`${id}-gauge`) as unknown as SVGPathElement;

        if (textElem && gaugeElem)
        {
            textElem.innerText = value.toFixed(1);

            // 0% -> offset 126, 100% -> offset 0
            const offset = this.maxDash - (value / 100) * this.maxDash;
            gaugeElem.style.strokeDashoffset = offset.toString();

            this.updateColor(gaugeElem, value);
        }
    }

    private updateColor(element: SVGPathElement, value: number): void
    {
        if (value > 80)
        {
            element.style.stroke = "var(--color-high)";
        }
        else if (value > 50)
        {
            element.style.stroke = "var(--color-medium)";
        }
        else
        {
            element.style.stroke = "var(--color-low)";
        }
    }
}

const monitorUI = new ResourceMonitor();
setInterval(() => monitorUI.tick(), 1000);