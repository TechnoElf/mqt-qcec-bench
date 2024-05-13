#!/usr/bin/env -S cargo -Zscript
---
[package]
name = "plot"
version = "0.1.0"
edition = "2021"

[dependencies]
plotters = "0.3"
csv = "1.3"
---

use plotters::prelude::*;

fn main() {
    let data: Vec<_> = csv::Reader::from_path("results.csv").unwrap().records().collect();
    let time: Vec<(String, f64, f64)> = data.iter().map(|r| r.as_ref().unwrap()).map(|r| (r[0].to_string(), r[1].to_string().parse().unwrap(), r[2].to_string().parse().unwrap())).collect();
    let nodes: Vec<(String, i64, i64)> = data.iter().map(|r| r.as_ref().unwrap()).map(|r| (r[0].to_string(), r[3].to_string().parse().unwrap(), r[4].to_string().parse().unwrap())).collect();

    let max_time = time.iter().map(|(_, a, b)| [*a, *b]).flatten().fold(0.0, f64::max) * 1.2;
    let max_nodes = nodes.iter().map(|(_, a, b)| [*a as f64, *b as f64]).flatten().fold(0.0, f64::max) * 1.2;

    let root = SVGBackend::new("time.svg", (1000, 600)).into_drawing_area();
    root.fill(&WHITE).unwrap();

    let mut chart = ChartBuilder::on(&root)
        .margin(10)
        .set_label_area_size(LabelAreaPosition::Bottom, 60)
        .set_label_area_size(LabelAreaPosition::Left, 60)
        .caption("QCEC Benchmark", ("sans-serif", 30))
        .build_cartesian_2d(0.0..(time.len() as f64), 0.0..max_time).unwrap();

    chart.configure_mesh()
        .x_label_formatter(&|x| format!("{}", time.get(x.floor() as usize).map(|(n, _, _)| n.as_str()).unwrap_or("")))
        .x_label_offset(60)
        .x_desc("Benchmark")
        .y_desc("Runtime (s)")
        .draw().unwrap();

    chart.draw_series(
        time.iter().enumerate().map(|(i, (_, diff, _))| {
            let x = i as f64;
            if *diff == 0.0 {
                Rectangle::new([(x, 0.0), (x + 0.5, max_time)], RED.mix(0.2).filled())
            } else {
                Rectangle::new([(x, 0.0), (x + 0.5, *diff)], RED.filled())
            }
        })
    ).unwrap().label("diff").legend(|(x, y)| Rectangle::new([(x, y - 5), (x + 10, y + 5)], RED.filled()));

    chart.draw_series(
        time.iter().enumerate().map(|(i, (_, _, proportional))| {
            let x = i as f64;
            if *proportional == 0.0 {
                Rectangle::new([(x + 0.5, 0.0), (x + 1.0, max_time)], BLUE.mix(0.2).filled())
            } else {
                Rectangle::new([(x + 0.5, 0.0), (x + 1.0, *proportional)], BLUE.filled())
            }
            
        })
    ).unwrap().label("proportional").legend(|(x, y)| Rectangle::new([(x, y - 5), (x + 10, y + 5)], BLUE.filled()));

    chart.configure_series_labels().background_style(&WHITE.mix(0.8)).border_style(&BLACK).draw().unwrap();

    let root = SVGBackend::new("nodes.svg", (1000, 600)).into_drawing_area();
    root.fill(&WHITE).unwrap();

    let mut chart = ChartBuilder::on(&root)
        .margin(10)
        .set_label_area_size(LabelAreaPosition::Bottom, 60)
        .set_label_area_size(LabelAreaPosition::Left, 60)
        .caption("QCEC Benchmark", ("sans-serif", 30))
        .build_cartesian_2d(0.0..(nodes.len() as f64), 0.0..max_nodes).unwrap();

    chart.configure_mesh()
        .x_label_formatter(&|x| format!("{}", nodes.get(x.floor() as usize).map(|(n, _, _)| n.as_str()).unwrap_or("")))
        .x_label_offset(60)
        .x_desc("Benchmark")
        .y_desc("Maximum Nodes (count)")
        .draw().unwrap();

    chart.draw_series(
        nodes.iter().enumerate().map(|(i, (_, diff, _))| {
            let x = i as f64;
            if *diff == 0 {
                Rectangle::new([(x, 0.0), (x + 0.5, max_nodes)], RED.mix(0.2).filled())
            } else {
                Rectangle::new([(x, 0.0), (x + 0.5, *diff as f64)], RED.filled())
            }
        })
    ).unwrap().label("diff").legend(|(x, y)| Rectangle::new([(x, y - 5), (x + 10, y + 5)], RED.filled()));

    chart.draw_series(
        nodes.iter().enumerate().map(|(i, (_, _, proportional))| {
            let x = i as f64;
            if *proportional == 0 {
                Rectangle::new([(x + 0.5, 0.0), (x + 1.0, max_nodes)], BLUE.mix(0.2).filled())
            } else {
                Rectangle::new([(x + 0.5, 0.0), (x + 1.0, *proportional as f64)], BLUE.filled())
            }

        })
    ).unwrap().label("proportional").legend(|(x, y)| Rectangle::new([(x, y - 5), (x + 10, y + 5)], BLUE.filled()));

    chart.configure_series_labels().background_style(&WHITE.mix(0.8)).border_style(&BLACK).draw().unwrap();
}
