-- 列出在 Athletics 场馆参赛的运动员信息
SELECT
    a.name AS athlete_name,                        -- 运动员名字
    a.country_code AS represented_country_code,    -- 代表的国家代码
    a.nationality_code AS nationality_country_code -- 国籍国家代码
FROM
    venues v
        JOIN
    results r
    ON v.venue = r.venue                           -- 比赛的场馆
        JOIN
    athletes a
    ON r.participant_code = a.code                 -- 比赛参与者是运动员
        OR r.participant_code IN (                     -- 或者参与者是团队
            SELECT code
            FROM teams
            WHERE athletes_code = a.code
        )
        JOIN
    countries c1
    ON c1.code = a.country_code                    -- 获取代表国家的坐标信息
        JOIN
    countries c2
    ON c2.code = a.nationality_code                -- 获取国籍国家的坐标信息
WHERE
    v.disciplines LIKE '%Athletics%'               -- 限制条件：场馆项目为 Athletics
  AND c1.Latitude IS NOT NULL                    -- 确保代表国家有经纬度
  AND c2.Latitude IS NOT NULL                    -- 确保国籍国家有经纬度
ORDER BY
    (POW(c1.Latitude - c2.Latitude, 2) +           -- 按距离降序排序（经纬度平方和）
     POW(c1.Longitude - c2.Longitude, 2)) DESC,
    athlete_name ASC;                              -- 距离相同则按名字升序排序
