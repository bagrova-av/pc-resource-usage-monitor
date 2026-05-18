"use strict";
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
class ResourceMonitor {
    constructor() {
        this.maxDash = 126; // The length of the full arc in SVG
    }
    tick() {
        return __awaiter(this, void 0, void 0, function* () {
            try {
                const response = yield fetch("/api/stats");
                const data = yield response.json();
                this.updateElement("cpu", data.cpuUsage);
                this.updateElement("mem", data.memUsage);
                const memDetails = document.getElementById("mem-abs");
                if (memDetails) {
                    memDetails.innerText = `${data.memUsed.toFixed(1)} / ${data.memTotal.toFixed(1)} GB`;
                }
            }
            catch (err) {
                console.error("Connection error");
            }
        });
    }
    updateElement(id, value) {
        const textElem = document.getElementById(`${id}-text`);
        const gaugeElem = document.getElementById(`${id}-gauge`);
        if (textElem && gaugeElem) {
            textElem.innerText = value.toFixed(1);
            // 0% -> offset 126, 100% -> offset 0
            const offset = this.maxDash - (value / 100) * this.maxDash;
            gaugeElem.style.strokeDashoffset = offset.toString();
            this.updateColor(gaugeElem, value);
        }
    }
    updateColor(element, value) {
        if (value > 80) {
            element.style.stroke = "var(--color-high)";
        }
        else if (value > 50) {
            element.style.stroke = "var(--color-medium)";
        }
        else {
            element.style.stroke = "var(--color-low)";
        }
    }
}
const monitorUI = new ResourceMonitor();
setInterval(() => monitorUI.tick(), 1000);
