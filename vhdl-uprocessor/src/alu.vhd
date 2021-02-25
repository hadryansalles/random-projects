---------------------------------------------------------------
-- file: alu.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 08/13/2020
-- brief: ALU operations 
---------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

-- ALU interface
entity alu is
    port (
        in0, in1 : in unsigned(15 downto 0);
        sel      : in unsigned(1 downto 0);
        CY       : out std_logic; -- '1' if a borrow occurs from MSB; otherwise, '0'.
        OV       : out std_logic; -- '1' if overflow occurs; otherwise, '0'.
        S        : out std_logic; -- '1' if the operation result is negative; otherwise, '0'.
        Z        : out std_logic; -- '1' if the operation result is '0'; otherwise, '0'
        data_out : out unsigned(15 downto 0); -- result
        higher_data_out : out unsigned(15 downto 0) -- higher 16bits of result
    );
end entity;

architecture a_alu of alu is
    -- signals
    signal out_sig: 
        unsigned(15 downto 0) := "0000000000000000";
    signal mul_res: 
        unsigned(31 downto 0) := "00000000000000000000000000000000";
    signal sum_res:
        unsigned(16 downto 0) := "00000000000000000";
    signal sub_res:
        unsigned(16 downto 0) := "00000000000000000";
    signal not_res:
        unsigned(15 downto 0) := "0000000000000000";
        
begin
    sum_res <= ('0' & in0) + ('0' & in1);
    sub_res <= ('0' & in0) - ('0' & in1);
    mul_res <= in0 * in1;
    not_res <= not in0; 

    -- mux all operations on output signal
    out_sig <= sum_res(15 downto 0) when sel = "00" else -- add
               sub_res(15 downto 0) when sel = "01" else -- sub
               mul_res(15 downto 0) when sel = "10" else -- mul
               not_res              when sel = "11" else -- not
               "0000000000000000";

    data_out <= out_sig;
    higher_data_out <= mul_res(31 downto 16);

    -- Flags
    CY <= sum_res(16) when sel = "00" else
          sub_res(16) when sel = "01" else
          '0';
    OV <= '1' when ((in0(15) = '1') and (in1(15) = '1') and (out_sig(15) = '0') and sel = "00") -- neg + neg = pos
                or ((in0(15) = '0') and (in1(15) = '0') and (out_sig(15) = '1') and sel = "00") -- pos + pos = neg
                or ((in0(15) = '1') and (in1(15) = '0') and (out_sig(15) = '0') and sel = "01") -- neg - pos = pos
                or ((in0(15) = '0') and (in1(15) = '1') and (out_sig(15) = '1') and sel = "01") -- pos - neg = neg
              else '0';
    S  <= out_sig(15);
    Z  <= '1' when out_sig = "0000000000000000" else '0';
end architecture;
