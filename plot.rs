#!/usr/bin/env -S cargo -Zscript
---
[dependencies]
plotters = "0.3"
csv = "1.3"
---

use plotters::prelude::*;

fn main() {
    let data: Vec<_> = csv::Reader::from_path("results.csv").unwrap().records().collect();
    let processed: Vec<(String, f64, f64)> = data.iter().map(|r| r.as_ref().unwrap()).map(|r| (r[0].to_string(), r[1].to_string().parse().unwrap(), r[2].to_string().parse().unwrap())).collect();

    println!("{:?}", processed);

    let root = SVGBackend::new("results.svg", (1000, 600)).into_drawing_area();
    root.fill(&WHITE).unwrap();

    let mut chart = ChartBuilder::on(&root)
        .margin(10)
        .set_label_area_size(LabelAreaPosition::Bottom, 60)
        .set_label_area_size(LabelAreaPosition::Left, 60)
        .caption("QCEC Benchmark", ("sans-serif", 30))
        .build_cartesian_2d(0.0..(processed.len() as f64), 0.0..15.0).unwrap();

    chart.configure_mesh()
        .x_label_formatter(&|x| format!("{}", processed.get(x.floor() as usize).map(|(n, _, _)| n.as_str()).unwrap_or("")))
        .x_label_offset(60)
        .x_desc("Benchmark")
        .y_desc("Runtime (s)")
        .draw().unwrap();

    chart.draw_series(
        processed.iter().enumerate().map(|(i, (name, diff, proportional))| {
            let x = i as f64;
            let val = if (*diff == 0.0) { 15.0 } else { *diff };
            Rectangle::new([(x, 0.0), (x + 0.5, val)], RED.filled())
        })
    ).unwrap().label("diff").legend(|(x, y)| Rectangle::new([(x, y - 5), (x + 10, y + 5)], RED.filled()));

    chart.draw_series(
        processed.iter().enumerate().map(|(i, (name, diff, proportional))| {
            let x = i as f64;
            let val = if (*proportional == 0.0) { 15.0 } else { *proportional };
            Rectangle::new([(x + 0.5, 0.0), (x + 1.0, val)], BLUE.filled())
        })
    ).unwrap().label("proportional").legend(|(x, y)| Rectangle::new([(x, y - 5), (x + 10, y + 5)], BLUE.filled()));

    chart.configure_series_labels().background_style(&WHITE.mix(0.8)).border_style(&BLACK).draw().unwrap();
}
